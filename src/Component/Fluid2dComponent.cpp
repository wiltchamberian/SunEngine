#include "Fluid2dComponent.h"
#include "Assets/AssetsMgr.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	Fluid2dComponent::Fluid2dComponent(int n, int m, float d, float t, float c, float mu ,float height, float refractive) {
        //high then ground
        float h = 0.01;
        p0_ = vec3(m*d, height, 0);
        p1_ = vec3(0, height, 0);
        p2_ = vec3(0, height, n*d);
        d_ = d;

		mu_ = mu;
        dt_ = t;
        c_ = c;
        n_ = n;
        m_ = m;
        height_ = height;
        refractive_ = refractive;
		
        vec3 normal = (p2_ - p1_).crossProduct(p0_ - p1_).getNormalized();

        vec3 dx = (p2_ - p1_) * (1.0 / n);
        vec3 dy = (p0_ - p1_) * (1.0 / m);

        std::vector<Vertex> vertices;
        std::vector<int> indices;
        for (int j = 0; j <= m; ++j) {
            for (int i = 0; i <= n; ++i) {
                vec3 p = p1_ + dx * i + dy * j;

                StaticMeshVertex vertex;
                vertex.pos = p;
                vertex.normal = normal;

                vertices.push_back(vertex);
            }
        }

        for (int j = 0; j < m; ++j) {
            for (int i = 0; i < n; ++i) {
                int left_bottom = j * (n + 1) + i;
                int right_bottom = left_bottom + 1;
                int left_top = (j + 1) * (n + 1) + i;
                int right_top = left_top + 1;

                indices.push_back(left_bottom);
                indices.push_back(right_bottom);
                indices.push_back(left_top);

                indices.push_back(left_top);
                indices.push_back(right_bottom);
                indices.push_back(right_top);
            }
        }
        setVertices(std::move(vertices));
        setIndices(std::move(indices));

        SharedPtr<PbrMaterialCom> mt(new PbrMaterialCom);
        materialCom_ = mt;
        mt->data_.albedo_ = vec4(40. / 255., 150. / 255., 200. / 255., 0.5);
        mt->data_.metallic_ = 0.01;
        mt->data_.roughness_ = 0.01;

        assert(c_ > 0);
        float tmp = c_ * 2 * dt_ / d_;
        assert(tmp * tmp < mu_* dt_ + 2);

        t1_ = (4 - 8 * c_ * c_ * dt_ * dt_ / (d_ * d_)) / (mu_ * dt_ + 2);
        t2_ = (mu_ * dt_ - 2) / (mu_ * dt_ + 2);
        t3_ = (2 * c_ * c_ * dt_ * dt_) / (d_ * d_ * (mu_ * dt_ + 2));

        zfieldsCur_.resize(getVerticesSize(), 0);
        zfieldsPre_.resize(getVerticesSize(), 0);
        zfieldsTmp_.resize(getVerticesSize(), 0);
        //初始化一个随机值
        //int a = 0;
        for (int j = 1; j < m_; ++j) {
            for (int i = 1; i < n_; ++i) {
                int k = j * (n_ + 1) + i;
                zfieldsPre_[k] = 0.5 * cos(A_TWICE_PI*j/m_)+1;
                zfieldsCur_[k] = 0.5 * sin(A_TWICE_PI *i / n_)+1;
            }
        }

        return;
	}

	void Fluid2dComponent::tick(float dtime, const SEvent& ev) {      
        static float cumTime = 0;
        cumTime += dtime;

        while (cumTime > dt_) {
            cumTime -= dt_;
            update();
        }
	}

    void Fluid2dComponent::update() {
        static int times = 0;
        //利用zfieldsCur 来更新静态网格的法线
        for (int j = 1; j < m_; ++j) {
            for (int i = 1; i < n_; ++i) {
                float z_i_1jk = zfieldsCur_[j * (n_ + 1) + i - 1];
                float z_i1jk = zfieldsCur_[j * (n_ + 1) + i + 1];
                float z_ij_1k = zfieldsCur_[(j - 1) * (n_ + 1) + i];
                float z_ij1k = zfieldsCur_[(j + 1) * (n_ + 1) + i];
                vertices_[j * (n_ + 1) + i].normal = vec3(z_ij_1k - z_ij1k, 2 * d_, z_i_1jk - z_i1jk);
            }
        }

        //int index = 0;
        for (int j = 1; j < m_; ++j) {
            for (int i = 1; i < n_; ++i) {
                float z_ijk = zfieldsCur_[j * (n_ + 1) + i];
                float z_ijk_1 = zfieldsPre_[j * (n_ + 1) + i];
                float z_i1jk = zfieldsCur_[j * (n_ + 1) + i + 1];
                float z_i_1jk = zfieldsCur_[j * (n_ + 1) + i-1];
                float z_ij1k = zfieldsCur_[(j + 1) * (n_ + 1) + i];
                float z_ij_1k = zfieldsCur_[(j - 1) * (n_ + 1) + i];
                zfieldsTmp_[j*(n_+1)+i] = t1_ * z_ijk + t2_ * z_ijk_1
                    + t3_ * (z_i1jk + z_i_1jk + z_ij1k + z_ij_1k);
                
            }
        }
        std::swap(zfieldsCur_, zfieldsPre_);
        std::swap(zfieldsCur_, zfieldsTmp_);
        times += 1;
        //计算方差f
        float sum = 0;
        for (int i = 0; i < zfieldsCur_.size(); ++i) {
            sum += zfieldsCur_[i];
        }
        sum = sum / zfieldsCur_.size();
        float sigma = 0;
        for (int i = 0; i < zfieldsCur_.size(); ++i) {
            sigma += (zfieldsCur_[i] - sum) * (zfieldsCur_[i] - sum);
        }
        sigma = sigma / zfieldsCur_.size();
        sigma = sqrt(sigma);
        if (sigma < 0.01) {
            assert(false);
        }
    }

    void Fluid2dComponent::updateVertexData() {
        g_graphicsLib->bufferSubData(EBufferType::ARRAY_BUFFER, 0, getVerticesSize() * sizeof(StaticMeshVertex), getVerticesData());
    }

    void Fluid2dComponent::bindUniformData(const ShaderProgram& pro) {
        unsigned int id = pro.getId();
        g_graphicsLib->setVec3(id, "pool.p0", p0_);
        g_graphicsLib->setVec3(id, "pool.p1", p1_);
        g_graphicsLib->setVec3(id, "pool.p2", p2_);
        g_graphicsLib->setFloat(id, "pool.height", height_);
        g_graphicsLib->setFloat(id, "pool.refractive", refractive_);
        if (hFluidTex.valid()) {
            g_graphicsLib->bindTexture(pro.getId(), hFluidTex.handle_, hFluidTex.textureType_, ETextureUnit::TEXTURE4, 4, "pool.albedoMap");
        }
    }

    void Fluid2dComponent::unBindUniformData(const ShaderProgram& pro) {
        if (hFluidTex.valid()) {
            g_graphicsLib->unBindTexture(pro.getId(),hFluidTex.handle_, hFluidTex.textureType_, ETextureUnit::TEXTURE4, 4, "pool.albedoMap");
        }
    }

}