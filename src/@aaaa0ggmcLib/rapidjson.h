#ifndef A_RAPIDJSON_H_INCLUDED
#define A_RAPIDJSON_H_INCLUDED
#include <string>
#include <vector>

#ifndef DLL_EXPORT
#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif
#endif // DLL_EXPORT

#ifdef __cplusplus
extern "C"
{
#endif

namespace alib{

    class DLL_EXPORT Document{
        struct Impl;
        Impl * data;
    public:
        struct DLL_EXPORT Member{
            std::string name;
            std::string value;
        };

        struct DLL_EXPORT Iterator{
        friend class  Document;
            Iterator(void *);
            ~Iterator();
        private:
            void * iter;
        };

        Document();
        ~Document();
        bool Parse(std::string data);
        std::string GetValue(std::string name);
        std::string GetValueI(std::vector<std::string> names);
        void ForEach(void (*fun)(std::string,std::string,void*),void* = NULL);
    };

}

#ifdef __cplusplus
}
#endif

#endif // A_RAPIDJSON_H_INCLUDED
