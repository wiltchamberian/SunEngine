#ifndef __SUN_UTIL_IRESOURCE_MGR_H
#define __SUN_UTIL_IRESOURCE_MGR_H

#include <unordered_map>
#include <optional>

namespace Sun {

    struct ResourceDefaultDeleter {
        template<typename _Resource>
        void operator() (_Resource& rc) {
            return;
        }
    };

    template<typename _Key, typename _Resource, typename _Deleter>
    class TResourceMgr {
    public:
        TResourceMgr(_Deleter deleter = ResourceDefaultDeleter())
        :deleter_(deleter){

        }
    protected:
        ~TResourceMgr() {
            for (auto& it : map_) {
                deleter_(it.second);
            }
        }

        void addResource(_Key key, _Resource rc) {
            map_.insert(std::make_pair(key, rc));
        }

        void removeResource(_Key key) {
            auto pr = map_.find(key);
            if (pr != map_.end()) {
                deleter_(pr->second);
                map_.erase(pr);
            }
        }

        std::optional<_Resource> getResource(_Key key) {
            auto it = map_.find(key);
            if (it != map_.end()) {
                return std::optional<_Resource>(it->second);
            }
            return std::optional<_Resource>();
        }
        _Deleter deleter_;
        std::unordered_map<_Key, _Resource> map_;
    };

}


#endif