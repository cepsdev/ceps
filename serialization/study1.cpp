#include <iostream>
#include <tuple>
#include <limits>

namespace ceps{
 namespace serialization{
   namespace basic{
     constexpr unsigned long long inline read_7bitrle_encoded(unsigned char* addr, unsigned short max_width = 10)
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
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) ;
        else if ( (r & 0x800000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) ;
        else if ( (r & 0x80000000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                                                                           ((r & 0x7f000000000000) >> 6);
        else if ( (r & 0x8000000000000000) == 0) 
         return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                                                                           ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7);
        else if ((*(addr + sizeof(unsigned long long int)) & 0x80) == 0) 
          return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                                                                            ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7) | 
                                                                            ((unsigned long long int)(*(addr + sizeof(unsigned long long int)) & 0x7f) << 56);
        return (r & 0x7f) | ((r & 0x7f00) >> 1) | ((r & 0x7f0000) >> 2) | ((r & 0x7f000000) >> 3) | ((r & 0x7f00000000) >> 4) | ((r & 0x7f0000000000) >> 5) | 
                                                                            ((r & 0x7f000000000000) >> 6) | ((r & 0x7f00000000000000) >> 7) | 
                                                                            ((unsigned long long int)(*(addr + sizeof(unsigned long long int)) & 0x7f) << 56)| 
                                                                            ((unsigned long long int)(*(addr + sizeof(unsigned long long int)+1) & 0x01) << 63);
     }
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
          //read--
          /*for(size_t j = 0; j != width;++j){
              unsigned char v = *( (unsigned char*)addr + j);
              stored_value |= (v & 0x7f) << j*7; 
              if (v & 0x80 == 0) break;
          }*/
          stored_value = basic::read_7bitrle_encoded((unsigned char*) addr,width);
          ++stored_value;
          unsigned short bytes_to_write = 0;
          if (0x8000000000000000 & stored_value){ // => 80 bit representation (64 bit value)
            bytes_to_write = 10;
          }
          else if (0x7f00000000000000 & stored_value){ // => 72 bit representation (63 bit value)
            bytes_to_write = 9;
          }
          else if (0xfe000000000000 & stored_value){ // => 64 bit representation (56 bit value)
            bytes_to_write = 8;
          }
          else if (  0x1fC0000000000 & stored_value){ // => 56 bit representation (49 bit value)
            bytes_to_write = 7;
          }
          else if (    0x3f800000000 & stored_value){ // => 48 bit representation (42 bit representation)
            bytes_to_write = 6;
          }
          else if (0x7f0000000 & stored_value){ // => 40 bit representation (35 bit representation)
            bytes_to_write = 5;
          }
          else if (0xfe00000 & stored_value){ // => 32 bit representation (28 bit representation)
            bytes_to_write = 4;
          }
          else if (0x1fc000 & stored_value){ // => 24 bit representation (21 bit representation)
            bytes_to_write = 3;
          }
          else if (0x7f80 & stored_value){ // => 16 bit representation (14 bit representation)
            bytes_to_write = 2;
          } else { // 8bit representation (7 bit value)
            bytes_to_write = 1;
          }
          for (unsigned short i = 0; i != bytes_to_write;++i,stored_value >>= 7){
              *addr = stored_value & 0x7F | (i + 1 != bytes_to_write ? 0x80:0);
              ++addr;
          }
          //std::cerr << "bytes_to_write:"<< bytes_to_write << std::endl;
          return bytes_to_write;
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

int main(){
    std::cout << "Serialization Study I\n";
    constexpr auto mem_size = 1024*1024;
    char* info_seg = new char[mem_size];
    char* type_seg = new char[mem_size];
    ceps::serialization::mem mem;
    //for (int i = 0; i <= 16384;++i)
    // mem.write_unchecked(i,false);
    //1  2  4  8  16  32  64  128 256 512 1024 2048 4096 8192  16384 32768  65536  131072  262144  524288  1048576
    //0  1  2  3  4   5   6   7   8   9   10   11   12   13    14    15     16     17      18      19      20
    //|-------- 0 - 127----| |----------128 -16383 ---------| | -------------- 16384 - 1048575 -------------------| 

    for (unsigned int i = 0; i <= 63;++i){
        mem.set_mem({info_seg,type_seg,mem_size,mem_size,0,0,0,0});
        for(unsigned long long j=0;j < 1 << i; ++j)
          mem.write_unchecked(j,false);
        std::cerr << "\nType Segment:2^"<< i << ":";
        mem.dump_type(std::cerr);
    }
    
    //std::cout << "Info Segment:\n";mem.dump_info(std::cout);std::cout << "\n\n";
    //std::cout << "Type Segment:\n";mem.dump_type(std::cout);
}