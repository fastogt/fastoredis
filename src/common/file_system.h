#pragma once

/**/

#include <fcntl.h>
#include <sys/stat.h>

#include "common/types.h"
#include "common/boost_extension.h"

namespace common
{
    namespace file_system
    {
        std::string get_file_name(std::string path);
        std::string get_dir_path(std::string path);
        tribool is_directory(const std::string& path);
        tribool is_file(const std::string& path);
        std::string stable_dir_path(std::string dir);
        std::string prepare_path(std::string path);

        inline char get_separator()
        {
            return '/';
        }

        class Path
        {
        public:
            Path();
            explicit Path(const std::string &path);
            Path(const Path &other);
            bool append(const std::string &path);
            std::string extension()const;
            bool is_valid()const;
            bool is_file()const;
            bool is_directory()const;
            const char* c_str() const;
            std::string directory() const;
            std::string path() const;

        private:
            tribool is_dir_;
            std::string path_;
        };

        Path make_path(const Path& p,const std::string &file_path);
        Path make_path_from_uri(const Path& p,const std::string &uri);

        inline bool operator <(const Path &lhs,const Path &rhs)
        {
            return lhs.path() < rhs.path();
        }

        inline bool operator ==(const Path &lhs,const Path &rhs)
        {
            return lhs.path() == rhs.path();
        }
    }

    namespace file_system
    {
        off_t get_file_size_by_descriptor(int fd_desc);
        bool clear_file_by_descriptor(int fd_desc) WARN_UNUSED_RESULT;
        bool get_flags_by_descriptor(int fd_desc, int &flags) WARN_UNUSED_RESULT;
        bool set_flags_by_descriptor(int fd_desc, int flags) WARN_UNUSED_RESULT;
        bool set_nonblocked_descriptor(int fd_desc);
        bool close_descriptor(int fd_desc) WARN_UNUSED_RESULT;
        bool open_descriptor(const std::string& path, int &fd_desc, int oflags) WARN_UNUSED_RESULT;
        bool open_descriptor(const std::string& path, int &fd_desc, int oflags, mode_t mode) WARN_UNUSED_RESULT;
#ifdef OS_POSIX
        bool create_node(const std::string &path) WARN_UNUSED_RESULT;
        bool create_node(const std::string& path, size_t permissions) WARN_UNUSED_RESULT;

        bool create_directory(const std::string& path, size_t permissions) WARN_UNUSED_RESULT;
#else
        bool create_node(const std::string &path) WARN_UNUSED_RESULT;
#endif
        bool create_directory(const std::string &path, bool isRecursive) WARN_UNUSED_RESULT;

        bool write_to_descriptor(int fd_desc, const void *buf, size_t len) WARN_UNUSED_RESULT;
        bool read_from_descriptor(int fd_desc, void *buf, size_t len, int &readlen) WARN_UNUSED_RESULT;

        class descriptor_owner
#ifdef BOOST_ENABLED
            : public boost_extension::intrusive_ptr_base<descriptor_owner>
#endif
        {
        public:
            descriptor_owner(int desc);
            ~descriptor_owner();

            int descriptor() const;
            off_t file_size()const;
            bool set_flags(int flags)const;
            bool flags(int &flags) const;

        private:
            const int descritor_;
        };

        class File
        {
        public:
            File(const Path& filePath);
            ~File();
            bool open(const char* mode);
            bool isOpened() const;
            bool read(buffer_type& outData, uint32_t maxSize);
            bool read(std::string& outData, uint32_t maxSize);
            bool readLine(buffer_type &outData);

            bool write(const buffer_type& data);
            bool write(const std::string& data);
            void flush();
            void close();

            bool isEof() const;

        private:
            Path path_;
            FILE* file_;
        };
    }
}
