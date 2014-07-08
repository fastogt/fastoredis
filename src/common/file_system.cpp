#include "common/file_system.h"

#include <string.h>
#include <sstream>

#include "common/url.h"
#include "common/logger.h"

#ifdef OS_POSIX
#include <linux/limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace common
{
    namespace file_system
    {
        bool clear_file_by_descriptor(int fd_desc)
        {
            bool result = false;
            if(fd_desc != INVALID_DESCRIPTOR){
                result = ftruncate(fd_desc,0)!=ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("ftruncate");
                }
            }

            return result;
        }

        bool get_flags_by_descriptor(int fd_desc, int &flags)
        {
            bool result = false;
            if(fd_desc != INVALID_DESCRIPTOR){
                result = (flags=fcntl(fd_desc,F_GETFL)) != ERROR_RESULT_VALUE;
                if(!result){
                   DEBUG_MSG_PERROR("get_flags_by_descriptor");
                }
            }

            return result;
        }

        bool set_flags_by_descriptor(int fd_desc, int flags)
        {
            bool result = false;
            if(fd_desc != INVALID_DESCRIPTOR){
                result = fcntl(fd_desc,F_SETFL,flags) != ERROR_RESULT_VALUE;
                if(!result){
                   DEBUG_MSG_PERROR("set_flags_by_descriptor");
                }
            }

            return result;
        }

        bool create_node(const unicode_string &path, size_t permissions)
        {
            if(path.empty()){
                return false;
            }

            bool result = mknod(path.c_str(), permissions,0) != ERROR_RESULT_VALUE;
            if(!result){
                DEBUG_MSG_PERROR("mknod");
            }

            return result;
        }

        bool open_descriptor(const unicode_string& path, int &fd_desc, int oflags, mode_t mode)
        {
            if(path.empty()){
                return false;
            }

            bool result = (fd_desc = open(path.c_str(), oflags, mode)) != ERROR_RESULT_VALUE;
            if(!result){
               DEBUG_MSG_PERROR("open_descriptor");
            }

            return result;
        }

        bool open_descriptor(const unicode_string& path, int &fd_desc, int oflags)
        {
            if(path.empty()){
                return false;
            }

            bool result = (fd_desc = open(path.c_str(), oflags)) != ERROR_RESULT_VALUE;
            if(!result){
               DEBUG_MSG_PERROR("open_descriptor");
            }

            return result;
        }

        bool close_descriptor(int fd_desc)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR)
            {
                result = close(fd_desc) != ERROR_RESULT_VALUE;
                if(!result)
                {
                   DEBUG_MSG_PERROR("close");
                }
            }
            return result;
        }

        bool set_nonblocked_descriptor(int fd_desc)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR)
            {
                int flags=0;
                if(get_flags_by_descriptor(fd_desc,flags))
                {
                    result = (flags&O_NONBLOCK)==O_NONBLOCK;
                    if(!result)
                    {
                        result = fcntl(fd_desc,F_SETFL,flags|O_NONBLOCK)!=ERROR_RESULT_VALUE;
                        if(!result)
                        {
                           DEBUG_MSG_PERROR("set_flags_by_descriptor");
                        }
                    }
                }
            }
            return result;
        }

        bool write_to_descriptor(int fd_desc,const void *buf,size_t len)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR)
            {
                result = write(fd_desc,buf,len)!=ERROR_RESULT_VALUE;
                if(!result)
                {
                    DEBUG_MSG_PERROR("write");
                }
            }
            return result;
        }

        bool read_from_descriptor(int fd_desc,void *buf,size_t len,int &readlen)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR)
            {
                readlen = read(fd_desc,buf,len);
                result = readlen!=ERROR_RESULT_VALUE;
                if(!result)
                {
                    unicode_perror("read");
                }
            }
            return result;
        }

        int descriptor_owner::descriptor() const
        {
            return descritor_;
        }

        off_t descriptor_owner::file_size()const
        {
            return get_file_size_by_descriptor(descritor_);
        }

        bool descriptor_owner::set_flags(int flags)const
        {
            return set_flags_by_descriptor(descritor_,flags);
        }

        bool descriptor_owner::flags(int &flags) const
        {
            return get_flags_by_descriptor(descritor_,flags);
        }

        descriptor_owner::~descriptor_owner()
        {
            close_descriptor(descritor_);
        }

        descriptor_owner::descriptor_owner(int desc)
            :descritor_(desc)
        {

        }

        off_t get_file_size_by_descriptor(int fd_desc)
        {
            off_t result = 0;
            if(fd_desc!=INVALID_DESCRIPTOR)
            {
                struct stat stat_buf;
                fstat(fd_desc, &stat_buf);
                result = stat_buf.st_size;
            }
            return result;
        }
    }
}
#endif
namespace common
{
    namespace file_system
    {
        unicode_string stable_dir_path(unicode_string path)
        {
            if(!path.empty())
            {
                path = prepare_path(path);
                size_t lenght = path.length();
                if (lenght>1&&path[lenght - 1] != get_separator())
                {
                    path+=get_separator();
                }
            }
            return path;
        }
        path make_path(const path& p,const unicode_string &file_path)
        {
            path result(p);
            result.append(file_path);
            return result;
        }
        path make_path_from_uri(const path& p, const unicode_string &uri)
        {
            path result;
            unicode_char *dec = url::detail::url_decode(uri.c_str());
            if(dec){
                result = make_path(p,dec);
                free(dec);
            }

            return result;
        }
        unicode_string get_dir_path(unicode_string path)
        {
            size_t pos = path.find_last_of(get_separator());
            if(pos!=unicode_string::npos)
            {
                path = stable_dir_path(path.substr(0,pos));
            }
            return path;
        }
        unicode_string prepare_path(unicode_string result)
        {
             if(!result.empty()&&result[0]=='~')
             {
             #ifdef OS_POSIX
                 char* home = getenv("HOME");
             #else
                 char* home = getenv("USERPROFILE");
             #endif
                 if(home){
                     unicode_string tmp=result;
                     result = home;
                     result += tmp.substr(1);
                 }
             }
             return result;
        }

        unicode_string get_file_name(unicode_string path)
        {
            size_t pos = path.find_last_of(get_separator());
            if(pos != unicode_string::npos)
            {
                path = path.substr(pos+1);
            }
            return path;
        }

        path::path()
            :is_dir_(INDETERMINATE)
        {

        }

        path::path(const unicode_string &path)
            :is_dir_(file_system::is_directory(path)),path_(is_directory()?stable_dir_path(path):path)
        {
        }

        path::path(const path &other):is_dir_(other.is_dir_),path_(other.path_)
        {

        }
        unicode_string path::extension()const
        {
            unicode_string ext;
            size_t pos = path_.find_first_of(UTEXT('.'));
            if(pos!=unicode_string::npos)
            {
                ext = path_.substr(pos+1);
            }
            return ext;
        }
        bool path::is_valid()const
        {
            return is_dir_!=INDETERMINATE;
        }
        bool path::is_file()const
        {
            return is_dir_==FAIL;
        }
        bool path::is_directory()const
        {
            return is_dir_==SUCCESS;
        }
        tribool is_directory(const unicode_string &path)
        {
            tribool result=INDETERMINATE;
            if(!path.empty())
            {
                struct stat filestat;
#ifdef OS_POSIX
                unicode_string p_path = prepare_path(path);
                if (::stat(p_path.c_str(), &filestat )!=ERROR_RESULT_VALUE)
                {
                    result = S_ISDIR(filestat.st_mode)?SUCCESS:FAIL;
                }
                else
                {
                     unicode_perror("stat");
                }
#else
#endif
            }
            return result;
        }
        tribool is_file(const unicode_string &path)
        {
            tribool result=INDETERMINATE;
            result = is_directory(path);
            if(result!=INDETERMINATE)
            {
                result = result==SUCCESS?FAIL:SUCCESS;
            }
            return result;
        }
        const unicode_char * path::c_str() const
        {
            return path_.c_str();
        }
        bool path::append(const unicode_string &path)
        {
            bool is_change=false;
            if(!path.empty())
            {
                if(path_.empty())
                {
                    path_ = path;
                    is_change=true;
                }
                else if(is_directory())
                {
                    path_+=get_file_name(path);
                    is_change=true;
                }
            }
            if(is_change)
            {
                is_dir_ = file_system::is_directory(path_);
                if(is_directory())
                {
                    path_ = stable_dir_path(path_);
                }
            }
            return is_change;
        }
    }
}
