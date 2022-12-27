#ifndef MGUTILITY_RANGE_HPP
#define MGUTILITY_RANGE_HPP

#include <type_traits>

namespace mgutility
{
  template <typename T = std::size_t>
class range {
 public:
   class iterator {
      friend class range;
    public:
      T operator *() const { return i_; }
      const iterator &operator ++() { i_ += ref_.incr_decr_; return *this; }
      iterator operator ++(int) { iterator copy(*this); i_ += ref_.incr_decr_; return copy; }
      bool operator ==(const iterator &other) const { return i_ == other.i_; }
      bool operator !=(const iterator &other) const { return i_ != other.i_; }

    protected:
      iterator(T start, range& r) : i_ (start), ref_(r) { }

    private:
      T i_;
      const range& ref_;
   };

   iterator begin() const { return begin_; }
   iterator end() const { return end_; }

   range(T  begin, T end, int inc_dec = 1) : begin_(begin, *this), end_(end, *this), 
   incr_decr_(begin < end ? inc_dec > 0 ? inc_dec : -inc_dec : inc_dec < 0 ? inc_dec : -inc_dec) 
   {
        static_assert(std::is_integral<T>::value, "range<T> requires integral type!");
        incr_decr_ = (incr_decr_ == 0 || incr_decr_ > end || -incr_decr_ > begin) ? 
        begin < end ? 1 : -1 : incr_decr_;
   }
private:
   iterator begin_;
   iterator end_;
   int incr_decr_;
};

template <typename T, typename ...Args>
range(T, Args...) -> range<T>;
  
}

#endif // MGUTILITY_RANGE_HPP
