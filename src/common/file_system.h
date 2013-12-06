#pragma once
#include "common/macros.h"
#include "common/boost_extension.hpp"
namespace common
{
    namespace file_system
    {
        unicode_string get_file_name(unicode_string path);
        unicode_string get_dir_path(unicode_string path);
        tribool is_directory(const unicode_string &path);
        tribool is_file(const unicode_string &path);
        unicode_string stable_dir_path(unicode_string dir);
        unicode_string prepare_path(const unicode_char *path);
        unicode_string prepare_path(unicode_string path);
        inline unicode_char get_separator()
        {
            return UTEXT('/');
        }
        bool url_decode(const unicode_string& in, unicode_char *ref);
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
            return unicode_strcmp(lhs.c_str(),rhs.c_str())==0;
        }
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
namespace std
{
    template<>
    struct hash<file_system::path>
            :public unary_function<file_system::path, size_t >
    {
        size_t operator()(const file_system::path &key)const
        {
            size_t hash = 0;
            const unicode_char* val=key.c_str();
            for (; *val; ++val)
            {
                hash+= *val;
            }
            return hash;
        }
    };
}
#endif
#ifdef OS_POSIX
    namespace file_system
    {
        off_t get_file_size_by_descriptor(ssize_t fd_desc);
        bool clear_file_by_descriptor(ssize_t fd_desc);
        bool get_flags_by_descriptor(ssize_t fd_desc,int &flags);
        bool set_flags_by_descriptor(ssize_t fd_desc,int flags);
        bool set_nonblocked_descriptor(ssize_t fd_desc);
        bool close_descriptor(ssize_t fd_desc);
        bool open_descriptor(const unicode_char* path,ssize_t &fd_desc,int oflags);
        bool open_descriptor(const unicode_char* path,ssize_t &fd_desc,int oflags,mode_t mode);
        bool create_node(const unicode_char* path,size_t permissions);
        bool write_to_descriptor(ssize_t fd_desc,const void *buf,size_t len);
        bool read_from_descriptor(ssize_t fd_desc,void *buf,size_t len,ssize_t &readlen);
        class descriptor_owner
#ifdef BOOST_SUPPORT_ENABLED
            : public boost_extension::intrusive_ptr_base<descriptor_owner>
#endif
        {
        public:
            descriptor_owner(ssize_t desc);
            ssize_t descriptor() const;
            off_t file_size()const;
            bool set_flags(int flags)const;
            bool flags(int &flags) const;
            ~descriptor_owner();
        private:
            const ssize_t descritor_;
        };
    }
#endif
}
