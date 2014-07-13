#pragma once

/**/

#include "common/types.h"
#include "common/boost_extension.h"

namespace common
{
    namespace file_system
    {
        unicode_string get_file_name(unicode_string path);
        unicode_string get_dir_path(unicode_string path);
        tribool is_directory(const unicode_string& path);
        tribool is_file(const unicode_string& path);
        unicode_string stable_dir_path(unicode_string dir);
        unicode_string prepare_path(unicode_string path);

        inline unicode_char get_separator()
        {
            return UTEXT('/');
        }

        class path
        {
        public:
            path();
            explicit path(const unicode_string &path);
            path(const path &other);
            bool append(const unicode_string &path);
            unicode_string extension()const;
            bool is_valid()const;
            bool is_file()const;
            bool is_directory()const;
            const unicode_char * c_str() const;

        private:
            tribool is_dir_;
            unicode_string path_;
        };

        path make_path(const path& p,const unicode_string &file_path);
        path make_path_from_uri(const path& p,const unicode_string &uri);

        inline bool operator <(const path &lhs,const path &rhs)
        {
            return unicode_strcmp(lhs.c_str(),rhs.c_str())<0;
        }

        inline bool operator ==(const path &lhs,const path &rhs)
        {
            return unicode_strcmp(lhs.c_str(),rhs.c_str()) == 0;
        }
    }
#ifdef OS_POSIX
    namespace file_system
    {
        off_t get_file_size_by_descriptor(int fd_desc);
        bool clear_file_by_descriptor(int fd_desc) WARN_UNUSED_RESULT;
        bool get_flags_by_descriptor(int fd_desc, int &flags) WARN_UNUSED_RESULT;
        bool set_flags_by_descriptor(int fd_desc, int flags) WARN_UNUSED_RESULT;
        bool set_nonblocked_descriptor(int fd_desc);
        bool close_descriptor(int fd_desc) WARN_UNUSED_RESULT;
        bool open_descriptor(const unicode_string& path, int &fd_desc, int oflags) WARN_UNUSED_RESULT;
        bool open_descriptor(const unicode_string& path, int &fd_desc, int oflags, mode_t mode) WARN_UNUSED_RESULT;
        bool create_node(const unicode_string& path, size_t permissions) WARN_UNUSED_RESULT;
        bool write_to_descriptor(int fd_desc,const void *buf, size_t len) WARN_UNUSED_RESULT;
        bool read_from_descriptor(int fd_desc,void *buf, size_t len, int &readlen) WARN_UNUSED_RESULT;

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
    }
#endif
}
