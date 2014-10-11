#include "common/file_system.h"

#include <string.h>
#include <sstream>

#include "common/url.h"
#include "common/logger.h"
#include "common/convert2string.h"

#include <unistd.h>

namespace
{
    using namespace common;
    bool create_directory_impl(const char* path)
    {
#ifdef OS_WIN
        bool result = mkdir(path) != ERROR_RESULT_VALUE;
#else
        bool result = mkdir(path, S_IRWXU|S_IRWXG|S_IRWXO) != ERROR_RESULT_VALUE;
#endif
        if(!result){
            DEBUG_MSG_PERROR("mkdir");
        }

        return result;
    }
}

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
#ifdef OS_POSIX
                result = (flags=fcntl(fd_desc,F_GETFL)) != ERROR_RESULT_VALUE;
                if(!result){
                   DEBUG_MSG_PERROR("get_flags_by_descriptor");
                }
#else
                #pragma message("IMPLEMENT PLZ")
#endif
            }

            return result;
        }

        bool set_flags_by_descriptor(int fd_desc, int flags)
        {
            bool result = false;
            if(fd_desc != INVALID_DESCRIPTOR){
#ifdef OS_POSIX
                result = fcntl(fd_desc,F_SETFL,flags) != ERROR_RESULT_VALUE;
                if(!result){
                   DEBUG_MSG_PERROR("set_flags_by_descriptor");
                }
#else
                #pragma message("IMPLEMENT PLZ")
#endif
            }

            return result;
        }
#ifdef OS_POSIX
        bool create_node(const std::string &path)
        {
            return create_node(path, S_IRWXU|S_IRWXG|S_IRWXO);
        }

        bool create_node(const std::string &path, size_t permissions)
        {
            if(path.empty()){
                return false;
            }

            bool result = mknod(path.c_str(), permissions, 0) != ERROR_RESULT_VALUE;
            if(!result){
                DEBUG_MSG_PERROR("mknod");
            }

            return result;
        }

        bool create_directory(const std::string& path, size_t permissions)
        {
            if(path.empty()){
                return false;
            }

            bool result = mkdir(path.c_str(), permissions) != ERROR_RESULT_VALUE;
            if(!result){
                DEBUG_MSG_PERROR("mkdir");
            }

            return result;
        }
#else
        bool create_node(const std::string &path)
        {
            if(path.empty()){
                return false;
            }
            #pragma message("IMPLEMENT PLZ")
            return false;
        }
#endif
        bool create_directory(const std::string& path, bool isRecursive)
        {
            if(path.empty()){
                return false;
            }

            std::string prPath = prepare_path(path);
            if(prPath[prPath.length() - 1] == get_separator()){
                prPath[prPath.length() - 1] = 0;
            }

            if(isRecursive){
                char *p = NULL;
#ifdef OS_WIN
                uint8_t shift = 3;
#else
                uint8_t shift = 1;
#endif
                for(p = const_cast<char*>(prPath.c_str() + shift); *p; p++ ){
                    if(*p == get_separator()){
                        *p = 0;
                        const char *path = prPath.c_str();

                        bool needCreate = false;
                        struct stat filestat;
                        if (::stat(path, &filestat) == ERROR_RESULT_VALUE){
                            needCreate = true;
                        }
                        else{
                            if(!S_ISDIR(filestat.st_mode)){
                                needCreate = true;
                            }
                        }

                        if(needCreate){
                            create_directory_impl(path);
                        }

                        *p = get_separator();
                    }
                }
                return create_directory_impl(prPath.c_str());
            }
            else{
                return create_directory_impl(prPath.c_str());
            }
        }

        bool open_descriptor(const std::string& path, int &fd_desc, int oflags, mode_t mode)
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

        bool open_descriptor(const std::string& path, int &fd_desc, int oflags)
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
            if(fd_desc!=INVALID_DESCRIPTOR){
                result = close(fd_desc) != ERROR_RESULT_VALUE;
                if(!result){
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
#ifdef OS_POSIX
                    result = (flags&O_NONBLOCK)==O_NONBLOCK;
                    if(!result)
                    {
                        result = fcntl(fd_desc,F_SETFL,flags|O_NONBLOCK)!=ERROR_RESULT_VALUE;
                        if(!result)
                        {
                           DEBUG_MSG_PERROR("set_flags_by_descriptor");
                        }
                    }
#else
                    #pragma message("IMPLEMENT PLZ")
#endif
                }
            }
            return result;
        }

        bool write_to_descriptor(int fd_desc,const void *buf,size_t len)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR){
                result = write(fd_desc,buf,len)!=ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("write");
                }
            }
            return result;
        }

        bool read_from_descriptor(int fd_desc,void *buf,size_t len,int &readlen)
        {
            bool result = false;
            if(fd_desc!=INVALID_DESCRIPTOR){
                readlen = read(fd_desc,buf,len);
                result = readlen!=ERROR_RESULT_VALUE;
                if(!result){
                    DEBUG_MSG_PERROR("read");
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
            bool res = close_descriptor(descritor_);
            DCHECK(res);
        }

        descriptor_owner::descriptor_owner(int desc)
            :descritor_(desc)
        {

        }

        off_t get_file_size_by_descriptor(int fd_desc)
        {
            off_t result = 0;
            if(fd_desc!=INVALID_DESCRIPTOR){
                struct stat stat_buf;
                fstat(fd_desc, &stat_buf);
                result = stat_buf.st_size;
            }
            return result;
        }
    }
}

namespace common
{
    namespace file_system
    {
        std::string stable_dir_path(std::string path)
        {
            if(!path.empty()){
                path = prepare_path(path);
                size_t lenght = path.length();
                if (lenght > 1 && path[lenght - 1] != get_separator()){
                    path += get_separator();
                }
            }
            return path;
        }
        Path make_path(const Path& p,const std::string &file_path)
        {
            Path result(p);
            result.append(file_path);
            return result;
        }
        Path make_path_from_uri(const Path& p, const std::string &uri)
        {
            Path result;
            char *dec = url::detail::url_decode(uri.c_str());
            if(dec){
                result = make_path(p,dec);
                free(dec);
            }

            return result;
        }
        std::string get_dir_path(std::string path)
        {
            size_t pos = path.find_last_of(get_separator());
            if(pos != std::string::npos){
                path = stable_dir_path(path.substr(0, pos));
            }
            return path;
        }
        std::string prepare_path(std::string result)
        {
             if(!result.empty()&&result[0]=='~'){
             #ifdef OS_POSIX
                 char* home = getenv("HOME");
             #else
                 char* home = getenv("USERPROFILE");
             #endif
                 if(home){
                     std::string tmp = result;
                     result = home;
                     result += tmp.substr(1);
                 }
             }
             std::replace(result.begin(), result.end(), '\\', get_separator());
             return result;
        }

        std::string get_file_name(std::string path)
        {
            size_t pos = path.find_last_of(get_separator());
            if(pos != std::string::npos){
                path = path.substr(pos+1);
            }
            return path;
        }

        Path::Path()
            :is_dir_(INDETERMINATE)
        {

        }

        Path::Path(const std::string &path)
            :is_dir_(file_system::is_directory(path)), path_(is_directory() ? stable_dir_path(path) : path)
        {
        }

        Path::Path(const Path &other)
            :is_dir_(other.is_dir_), path_(other.path_)
        {

        }

        std::string Path::extension()const
        {
            std::string ext;
            size_t pos = path_.find_first_of('.');
            if(pos!=std::string::npos){
                ext = path_.substr(pos+1);
            }
            return ext;
        }

        bool Path::is_valid()const
        {
            return is_dir_ != INDETERMINATE;
        }

        bool Path::is_file()const
        {
            return is_dir_ == FAIL;
        }

        bool Path::is_directory()const
        {
            return is_dir_ == SUCCESS;
        }

        tribool is_directory(const std::string &path)
        {
            tribool result = INDETERMINATE;
            if(!path.empty()){
                struct stat filestat;
                std::string p_path = prepare_path(path);
                if (::stat(p_path.c_str(), &filestat ) != ERROR_RESULT_VALUE){
                    result = S_ISDIR(filestat.st_mode) ? SUCCESS:FAIL;
                }
                else{
                     DEBUG_MSG_PERROR("stat");
                }
            }
            return result;
        }
        tribool is_file(const std::string &path)
        {
            tribool result=INDETERMINATE;
            result = is_directory(path);
            if(result != INDETERMINATE){
                result = result == SUCCESS ? FAIL:SUCCESS;
            }
            return result;
        }

        std::string Path::path() const
        {
            return path_;
        }

        const char* Path::c_str() const
        {
            return path_.c_str();
        }

        bool Path::append(const std::string &path)
        {
            bool is_change=false;
            if(!path.empty()){
                if(path_.empty()){
                    path_ = path;
                    is_change=true;
                }
                else if(is_directory()){
                    path_ += get_file_name(path);
                    is_change=true;
                }
            }
            if(is_change){
                is_dir_ = file_system::is_directory(path_);
                if(is_directory()){
                    path_ = stable_dir_path(path_);
                }
            }
            return is_change;
        }

        std::string Path::directory() const
        {
            return get_dir_path(path_);
        }

        File::File(const Path &filePath)
            : path_(filePath), file_(NULL)
        {

        }

        File::~File()
        {
            close();
        }

        bool File::open(const char* mode)
        {
            if(!file_){
                file_ = fopen(path_.c_str(), mode);
                return file_;
            }

            return true;
        }

        bool File::read(buffer_type& outData, uint32_t maxSize)
        {
            if(!file_){
                return false;
            }

            byte_type* data = (byte_type*)calloc(maxSize, sizeof(byte_type));
            if(!data){
                return false;
            }

            size_t res = fread(data, sizeof(byte_type), maxSize, file_);
            if(res > 0){
                outData = buffer_type(data, res);
            }
            else if(feof(file_)){

            }
            free(data);

            return true;
        }

        bool File::read(std::string& outData, uint32_t maxSize)
        {
           buffer_type outB;
           bool res = read(outB, maxSize);
           if(res){
               outData = convertToString(outB);
           }
           return res;
        }

        bool File::readLine(buffer_type& outData)
        {
            if(!file_){
                return false;
            }

            char buff[1024] = {0};

            char* res = fgets(buff, sizeof(buff), file_);
            if(res){
                outData = convertFromString<buffer_type>(buff);
            }

            return true;
        }

        bool File::isEof() const
        {
            if(!file_){
                return true;
            }
            return feof(file_);
        }

        bool File::write(const buffer_type& data)
        {
            if(!file_){
                return false;
            }

            if(!data.length()){
                NOTREACHED();
                return false;
            }

            size_t res = fwrite(data.c_str(), sizeof(byte_type), data.length(), file_);
            if(res != data.length()){
                DEBUG_MSG_PERROR("write");
            }
            return res == data.length();
        }

        bool File::write(const std::string& data)
        {
            return write(convertFromString<buffer_type>(data));
        }

        bool File::write(const string16& data)
        {
            return write(convertFromString16<buffer_type>(data));
        }

        void File::flush()
        {
            if(!file_){
                return;
            }
            fflush(file_);
        }

        bool File::isOpened() const
        {
            return file_;
        }

        void File::close()
        {
            if(file_){
                fclose(file_);
                file_ = NULL;
            }
        }
    }
}
