#include <iostream>
#include <iomanip>
#include <tuple>
#include <limits>
#include <cstring>
#include <cassert>

namespace ceps{
 namespace serialization{
   namespace basic{
     constexpr unsigned long long inline read_base128_ull(unsigned char* addr, unsigned short max_width = 10)
     {
        unsigned long long r = *((unsigned long long*)addr);
        if ( (r & 0x80) == 0) return r & 0x7f;
        else if ( (r & 0x8000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1);
        else if ( (r & 0x800000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2);
        else if ( (r & 0x80000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) ;
        else if ( (r & 0x8000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
                ((r & 0x7f00000000) >> 4) ;
        else if ( (r & 0x800000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
                ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) ;
        else if ( (r & 0x80000000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
                ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | ((r & 0x7f000000000000) >> 6);
        else if ( (r & 0x8000000000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
                ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7);
        else if ((*(addr + sizeof(unsigned long long int)) & 0x80) == 0) 
          return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
                 ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                 ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7) | 
                 ((unsigned long long int)(*(addr + sizeof(unsigned long long int)) & 0x7f) << 56);
        return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | 
               ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
               ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7) | 
               ((unsigned long long int)(*(addr + sizeof(unsigned long long int)) & 0x7f) << 56)| 
               ((unsigned long long int)(*(addr + sizeof(unsigned long long int)+1) & 0x01) << 63);
     }
   }
   unsigned short write_base128(unsigned char* addr, unsigned long long value){
          unsigned short bytes_to_write = 0;
          if (0x8000000000000000 & value){ // => 80 bit representation (64 bit value)
            bytes_to_write = 10;
          }
          else if (0x7f00000000000000 & value){ // => 72 bit representation (63 bit value)
            bytes_to_write = 9;
          }
          else if (0xfe000000000000 & value){ // => 64 bit representation (56 bit value)
            bytes_to_write = 8;
          }
          else if (  0x1fC0000000000 & value){ // => 56 bit representation (49 bit value)
            bytes_to_write = 7;
          }
          else if (    0x3f800000000 & value){ // => 48 bit representation (42 bit representation)
            bytes_to_write = 6;
          }
          else if (0x7f0000000 & value){ // => 40 bit representation (35 bit representation)
            bytes_to_write = 5;
          }
          else if (0xfe00000 & value){ // => 32 bit representation (28 bit representation)
            bytes_to_write = 4;
          }
          else if (0x1fc000 & value){ // => 24 bit representation (21 bit representation)
            bytes_to_write = 3;
          }
          else if (0x7f80 & value){ // => 16 bit representation (14 bit representation)
            bytes_to_write = 2;
          } else { // 8bit representation (7 bit value)
            bytes_to_write = 1;
          }
          for (unsigned short i = 0; i != bytes_to_write;++i,value >>= 7){
              *addr = value & 0x7F | (i + 1 != bytes_to_write ? 0x80:0);
              ++addr;
          }
          return bytes_to_write;
   }
   
   unsigned short write_base128(unsigned char* addr, unsigned int value){
          return write_base128(addr, (unsigned long long) value);
   }

   unsigned short write_base128(unsigned char* addr, unsigned short value){
          return write_base128(addr, (unsigned long long) value);
   }

   unsigned short write_base128(unsigned char* addr, unsigned char value){
          return write_base128(addr, (unsigned long long) value);
   }

  template<typename T> struct mem_tag{      

  };
  template<> struct mem_tag<std::int32_t>{
     using tag_t = signed char; 
     static const tag_t tag = 5; 
     static const tag_t rle_tag = -5;
     static const size_t tag_len = sizeof(tag_t);  
  };
   
  struct tag_match_result_t{
      operator bool(){return match;}
      bool match; 
      bool rle;
  };
  
  template<typename T> struct type_handler:public mem_tag<T>{
      static size_t write_tag(char* addr,bool rle = false){
          *((typename mem_tag<T>::tag_t*)addr) = rle?mem_tag<T>::rle_tag:mem_tag<T>::tag;
          return mem_tag<T>::tag_len;
      }
      static void write_length_7bit_encoded(char* addr,int v){
          *addr = v;
      }
      static size_t increment_7bit_encoded(char* addr,size_t width){
          unsigned long long int stored_value = 0;
          stored_value = basic::read_base128_ull((unsigned char*) addr,width);
          ++stored_value;
          return  write_base128( (unsigned char*)addr, stored_value);
      }
      static tag_match_result_t tag_match(char* addr){
          if (*addr == mem_tag<T>::tag || *addr == mem_tag<T>::rle_tag)
           return{true,*addr == mem_tag<T>::rle_tag};
          return {false,false}; 
      }       
  };

  class mem{
      public:
        struct seg_info{
            char* type_seg_          = {};
            char* info_seg_          = {};
            size_t type_seg_size_    = {};
            size_t info_seg_size_    = {};
            size_t type_seg_ofs_     = {};
            size_t info_seg_ofs_     = {};
            size_t info_seg_written_ = {};
            size_t type_seg_written_ = {};
        };
        using set_mem_result_t = seg_info;
        using write_result_t = std::pair<size_t,size_t>;
      private:
        seg_info segs_ = {};
      public:
        set_mem_result_t set_mem(seg_info new_seg_info){
            auto t = segs_;
            segs_ = new_seg_info;
            return segs_;
        }
        write_result_t write_unchecked(std::int32_t v,bool write_through = true){
            //Write Data
            if (write_through) *((decltype(v)*) (segs_.info_seg_+segs_.info_seg_written_) ) = v;
            segs_.info_seg_written_ += sizeof(v);
            //Write Type
            if (segs_.type_seg_ofs_ == segs_.type_seg_written_){
                //Start of a data
                segs_.type_seg_written_ += type_handler<decltype(v)>::write_tag(segs_.type_seg_ + segs_.type_seg_ofs_);
            } else if (auto r = type_handler<decltype(v)>::tag_match(segs_.type_seg_ + segs_.type_seg_ofs_)) {
                if(!r.rle) {
                    type_handler<decltype(v)>::write_tag(segs_.type_seg_ + segs_.type_seg_ofs_,true);
                    type_handler<decltype(v)>::write_length_7bit_encoded(segs_.type_seg_ + segs_.type_seg_ofs_ + 1,2);
                    ++segs_.type_seg_written_;
                } else {
                    auto cur_width_of_len_tag = segs_.type_seg_written_ - segs_.type_seg_ofs_ - 1;
                    auto cur_width = type_handler<decltype(v)>::increment_7bit_encoded(segs_.type_seg_ + segs_.type_seg_ofs_ + 1,segs_.type_seg_written_ - segs_.type_seg_ofs_ - 1);
                    if (cur_width > cur_width_of_len_tag) segs_.type_seg_written_ += cur_width - cur_width_of_len_tag;
                }            
            }            
            return {};
        }
        std::ostream& dump_info(std::ostream& os){
            for(size_t i = 0;i != segs_.info_seg_written_;++i){
             if (i % 16 == 0) os << "\n";
             os << (int)*( ((unsigned char *)segs_.info_seg_)+i) << ' ';
            }
            return os;
        }
        std::ostream& dump_type(std::ostream& os){
            for(size_t i = 0;i != segs_.type_seg_written_;++i){
             if (i % 16 == 0) os << "\n";
             os << (int)*( (unsigned char *) segs_.type_seg_+i) << ' ';
            }
            return os;
        }

  };
 }
}


namespace ceps{
 namespace serialization{
   namespace basic{
     namespace tests{
       void dump_data(unsigned char* buffer, size_t elems){
         for(size_t i = 0; i != elems;++i)
          std::cout << std::setw(4) << std::setfill(' ') << (unsigned int)buffer[i] << ((i+1) % 16 == 0 ? "\n":" ");
       }
       void read_write_128_base(){
         constexpr auto buf_size = 128;
         unsigned char buffer[buf_size];// = {0};
         auto read_write_128_base_step = [&](unsigned long long v){
                    memset(buffer,0,buf_size); 
                    auto wr = write_base128(buffer, v);
                    auto rr = read_base128_ull(buffer);
                    if (v!=rr){ 
                      std::cout << "bytes written:"<< wr << " value read:" << rr << " value expected: " << v << "\n";
                      dump_data(buffer,buf_size);
                      assert(v==rr);
                    }
         };
            //1  2  4  8  16  32  64  128 256 512 1024 2048 4096 8192  16384 32768  65536  131072  262144  524288  1048576
            //0  1  2  3  4   5   6   7   8   9   10   11   12   13    14    15     16     17      18      19      20
            //|-------- 0 - 127----| |----------128 -16383 ---------| | -------------- 16384 - 1048575 -------------------| 

         for (auto v : {0ull,1ull,127ull,128ull,129ull,16383ull,16384ull,32768ull,1048676ull,1048677ull,
                        0x0ull,0xfull,0xffull,0xfffull,0xffffull,0xfffffull,0xffffffull,0xfffffffull,0xffffffffull,0xfffffffffull,0xffffffffffull,
                        0xfffffffffffull,0xffffffffffffull,0xfffffffffffffull,0xffffffffffffffull,0xfffffffffffffffull, 0xffffffffffffffffull,
                        0x1ull,0xeull,0xfeull,0xefull,0xfffull,0xeffull,0xaffull,0xbf3ull,0xffffull,0xfffffull,0xffffffull,0xfffffffull,0xffffffffull,
                        0xfffffffffull,0xffffffffffull,0xfffffffffffull,0xffffffffffffull,0xfffffffffffffull,0xffffffffffffffull,0xfffffffffffffffull, 
                        0xffffffffffffffffull,
                         }
                        )
           read_write_128_base_step(v);
       }
       void run_all(){
         read_write_128_base();
       }
     }
   }
  }
}

int main(){
    ceps::serialization::basic::tests::run_all();
}