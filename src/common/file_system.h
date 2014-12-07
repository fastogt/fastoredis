#pragma once

/**/

#include "common/types.h"

namespace common
{
    namespace file_system
    {
        std::string prepare_path(std::string path);

        tribool is_directory(const std::string& path);

        template<typename CharT>
        inline CharT get_separator()
        {
            return '/';
        }

        template<typename CharT>
        inline std::basic_string<CharT> get_file_name(std::basic_string<CharT> path)
        {
            size_t pos = path.find_last_of(get_separator<CharT>());
            if(pos != std::basic_string<CharT>::npos){
                path = path.substr(pos+1);
            }
            return path;
        }

        template<typename CharT>
        inline std::basic_string<CharT> stable_dir_path(std::basic_string<CharT> path)
        {
            if(!path.empty()){
                path = prepare_path(path);
                size_t lenght = path.length();
                if (lenght > 1 && path[lenght - 1] != get_separator<CharT>()){
                    path += get_separator<CharT>();
                }
            }
            return path;
        }

        template<typename CharT>
        inline std::basic_string<CharT> get_dir_path(std::basic_string<CharT> path)
        {
            size_t pos = path.find_last_of(get_separator<CharT>());
            if(pos != std::basic_string<CharT>::npos){
                path = stable_dir_path(path.substr(0, pos));
            }
            return path;
        }

        template<typename CharT>
        tribool is_file(const std::basic_string<CharT>& path)
        {
            tribool result = INDETERMINATE;
            result = is_directory(path);
            if(result != INDETERMINATE){
                result = result == SUCCESS ? FAIL : SUCCESS;
            }
            return result;
        }

        off_t get_file_size_by_descriptor(int fd_desc);
        bool clear_file_by_descriptor(int fd_desc) WARN_UNUSED_RESULT;\
        bool close_descriptor(int fd_desc) WARN_UNUSED_RESULT;

        bool open_descriptor(const std::string& path, int &fd_desc, int oflags) WARN_UNUSED_RESULT;
#ifdef OS_POSIX
        bool create_node(const std::string &path) WARN_UNUSED_RESULT;
        bool create_node(const std::string& path, size_t permissions) WARN_UNUSED_RESULT;
#else
        bool create_node(const std::string &path) WARN_UNUSED_RESULT;
#endif
        bool create_directory(const std::string &path, bool isRecursive) WARN_UNUSED_RESULT;

        bool write_to_descriptor(int fd_desc, const void *buf, unsigned int len) WARN_UNUSED_RESULT;
        bool read_from_descriptor(int fd_desc, void *buf, unsigned int len, int &readlen) WARN_UNUSED_RESULT;

        bool findFileInPath(const char* fileName, std::string &outPath) WARN_UNUSED_RESULT;

        //==============================Path=====================================//

        class Path
        {
        public:
            Path();
            explicit Path(const std::string &path);
            Path(const Path &other);

            bool append(const std::string &path);
            std::string extension()const;
            bool isValid()const;
            bool isFile()const;
            bool isDirectory()const;

            std::string directory() const;
            std::string path() const;

        private:
            tribool is_dir_;
            std::string path_;
        };

        Path make_path(const Path& p, const std::string &file_path);
        Path make_path_from_uri(const Path& p, const std::string &uri);

        inline bool operator <(const Path &lhs,const Path &rhs)
        {
            return lhs.path() < rhs.path();
        }

        inline bool operator ==(const Path &lhs,const Path &rhs)
        {
            return lhs.path() == rhs.path();
        }

        //==============================File=====================================//

        class File
        {
        public:
            File(const Path& filePath);
            ~File();

            bool open(const char* mode) WARN_UNUSED_RESULT;
            bool isOpened() const;
            bool read(buffer_type& outData, uint32_t maxSize);
            bool read(std::string& outData, uint32_t maxSize);
            bool readLine(buffer_type &outData);

            bool write(const buffer_type& data);
            bool write(const std::string& data);
            bool write(const common::string16& data);

            void flush();
            void close();

            bool isEof() const;

        private:
            Path path_;
            FILE* file_;
        };
    }
}
