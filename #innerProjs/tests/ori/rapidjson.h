#ifndef A_RAPIDJSON_H_INCLUDED
#define A_RAPIDJSON_H_INCLUDED
#include <string>
#include <vector>

namespace alib{

    class Document{
        struct Impl;
        Impl * data;
    public:
        struct Member{
            std::string name;
            std::string value;
        };

        struct Iterator{
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
#endif // A_RAPIDJSON_H_INCLUDED
