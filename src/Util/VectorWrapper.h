#ifndef __SUN_UTIL_VECTOR_WRAPPER_H
#define __SUN_UTIL_VECTOR_WRAPPER_H

#include <vector>

namespace Sun {

    //抽象编程，这个类是vector的一种wraper,现实中，经常需要实现这的类:
    /*
    class XXX{
        //....
        std::vector<_T> vec_;
    };
    如此，就需要 wrapper很多接口，将这部分功能抽象到这个wrapper中，使用者只需继承该类即可
    */
    template<typename _T>
    class TVectorWrapper : public std::vector<_T> {
    public:
        TVectorWrapper(std::vector<_T>& vec) :vec_(vec) {}
        void addElement(const _T& ele) {
            vec_.emplace_back(ele);
        }
        void removeElement(const _T& ele) {
            for (auto it = vec_.begin(); it != vec_.end();++it) {
                if (*it == ele) {
                    vec_.erase(it);
                    break;
                }
            }
            return;
        }
        void setElements(const std::vector<_T>& vec) {
            vec_ = vec;
        }
        void addElements(const std::vector<_T>& vec) {
            for (auto& it : vec) {
                vec_.emplace_back(it);
            }
        }
        void clearElements() {
            vec_.clear();
        }

    protected:
        std::vector<_T>& vec_;
    };

}

#endif 