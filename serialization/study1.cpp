#include <iostream>
#include <tuple>

namespace ceps{
 namespace serialization{
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
      static void increment_7bit_encoded(char* addr,size_t width){
          unsigned long long int stored_value = 0;
          //read
          for(size_t j = 0; j != width;++j){
              unsigned char v = *( (unsigned char*)addr + j);
              stored_value |= (v & 0x7f) << j*7; 
              if (v & 0x80 == 0) break;
          }
          ++stored_value;
          unsigned short bytes_to_write = 0;
          if (0x7f80 & stored_value){ // => 16 bit representation (14 bit representation)
            bytes_to_write = 2;
          } else { // 8bit representation (7 bit value)
            bytes_to_write = 1;
          }
          for (unsigned short i = 0; i != bytes_to_write;++i,stored_value >> 7){
              *addr = stored_value & 0x7F | (i + 1 != bytes_to_write ? 0x80:0);
              ++addr;
          }

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
        write_result_t write_unchecked(std::int32_t v){
            //Write Data
            *((decltype(v)*) (segs_.info_seg_+segs_.info_seg_written_) ) = v;
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
                    type_handler<decltype(v)>::increment_7bit_encoded(segs_.type_seg_ + segs_.type_seg_ofs_ + 1,segs_.type_seg_written_ - segs_.type_seg_ofs_ - 1);
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
             os << (int)*(segs_.type_seg_+i) << ' ';
            }
            return os;
        }

  };
 }
}

int main(){
    std::cout << "Serialization Study I\n";
    char info_seg[1024];
    char type_seg[1024];
    ceps::serialization::mem mem;
    mem.set_mem({info_seg,type_seg,1024,1024,0,0});
    for (int i = 0; i != 128;++i)
     mem.write_unchecked(i);
    std::cout << "Info Segment:\n";mem.dump_info(std::cout);std::cout << "\n\n";
    std::cout << "Type Segment:\n";mem.dump_type(std::cout);
    
}