#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <utility>
#include <ostream>
#include <stdexcept>

using namespace std;

template <typename T> class Vector
{
	public:
	class ConstIterator;
	class Iterator;
	using size_type = std::size_t;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;


	private:
	static constexpr size_type min_sz {5}; 
	size_type sz;
	size_type max_sz;
	value_type* values;


	public:
	Vector() : Vector(this->min_sz)
    {}

	Vector(size_type n)
	{
		if(n<min_sz)
			n = this->min_sz;
		this->sz = 0;
		this->values = nullptr;
		this->max_sz = 0;		
		reserve(n);
	}

	Vector(const Vector& src)
	{
		this->max_sz = src.max_sz;
		this->values = new value_type[this->max_sz];
		for(size_type i{0}; i<src.sz; ++i)
			this->values[i] = src.values[i];	
		this->sz = src.sz;
	}

	Vector(initializer_list<value_type> list)
	{
		this->sz = list.size();
		this->max_sz = this->sz*2+1;
		this->values = new value_type[this->max_sz];
		size_type i {0};
		for(const auto &temp : list)
			this->values[i++] = temp;
	}

	~Vector()
    {
        delete[] values;
    }


	void reserve(size_type n)
	{
		if(n > this->max_sz) 
		{
			size_type new_sz = n;
			value_type* new_values = new value_type[new_sz];
			for(size_type i{0}; i<this->sz; ++i)
				new_values[i] = this->values[i];	
			this->max_sz = new_sz;
			delete[] values;
			this->values = new_values;
		}
	}

	void push_back(const_reference val)
	{
		if(this->sz >= this->max_sz)
		{
			reserve(this->max_sz*2);
		}
		this->values[sz++]=val;
	}

	size_type size() const
	{
		return this->sz;
	}

	bool empty() const
	{
		if(this->sz <= 0) return true;
		return false;
	}

	void clear() 
	{
		sz = 0;
	}

	void shrink_to_fit()
	{
		if(sz<max_sz && max_sz>5)
		{
			size_type new_sz {5};
			if(sz > 5) {new_sz = sz;}
			value_type* values_temp = new value_type[new_sz];
			for(size_type i{0}; i<sz; i++)
				values_temp[i] = this->values[i];
			delete[] this->values;
			this->values = values_temp;
			this->max_sz = new_sz;										
		}
	}

	void pop_back()
	{
		if(this->empty()) throw runtime_error("Vector ist leer!");
		this->sz--;
	}

	size_type capacity() const
	{
		return max_sz;
	}

	iterator begin()
	{
		return iterator {this->values, this};
	}

	iterator end()
	{
		return iterator {this->values+this->sz, this};
	}

	const_iterator begin() const
	{
		return const_iterator {this->values, this};
	}

	const_iterator end() const
	{
		return const_iterator {this->values+this->sz, this};
	}

	iterator insert(const_iterator pos, const_reference val) 
	{
		auto diff = pos-this->begin();
		
		if(diff<0 || static_cast<size_type>(diff)>this->sz)
			throw std::runtime_error("Iterator out of bounds");
		
		size_type current {static_cast<size_type>(diff)};
		if(this->sz >= this->max_sz)
			reserve(this->max_sz*2);
		for(auto i {sz}; i-->current; )
			values[i+1] = values[i];
		values[current] = val;
		++sz;
		return iterator {this->values+current, this};
	}

	iterator erase(const_iterator pos) 
	{
		auto diff = pos-begin();
		
		if(diff<0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		
		size_type current {static_cast<size_type>(diff)};
		for(auto i {current}; i<sz-1; ++i)
			values[i] = values[i+1];
		--sz;
		return iterator {values+current, this};
	}


	Vector operator=(Vector src)
	{
		swap(this->sz, src.sz);
		swap(this->max_sz, src.max_sz);
		swap(this->values, src.values);
		return src;
	}

	reference operator[](size_type index) 
	{
		if(index<0 || index>=sz) throw runtime_error ("out of range");
		return this->values[index];
	}

	const_reference operator[](size_type index) const
	{
		if(index<0 || index>=sz) throw runtime_error ("out of range");
		return this->values[index];
	}

	friend ostream& operator<<(ostream& os, const Vector& v)		
	{
		os << '[';
		for(Vector::
		size_type i{0}; i<v.sz; i++){
			os << v.values[i];
			if(i<v.sz-1)
				os << ", ";
		}
		os << ']';
		return os;
	}

		
	//Iteratoren
	class Iterator 
	{
		public:
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
		using const_iterator = Vector::const_iterator;
		using iterator = Vector::Iterator;
		using size_type = Vector::size_type;
		using v_pointer = Vector*;


		private:
		pointer ptr;
		v_pointer v;


		public:
		Iterator()
		{
			this->ptr = nullptr;
			this->v = nullptr;
		}

		Iterator(pointer ptr, v_pointer v = nullptr)
		{
			this->ptr=ptr;
			this->v=v;
		}

		reference operator*() const
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				throw runtime_error {"1"};
			return *this->ptr;	//von ptr referenzierter Wert
		}

		pointer operator->() const
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				throw runtime_error {"1"};
			return this->ptr;
		}

		bool operator==(const const_iterator& it) const
		{
			return static_cast<ConstIterator>(*this)==it;
		}

		bool operator!=(const const_iterator& it) const
		{
			return (!(*this == it));
		}

		operator const_iterator() const	
		{
			return ConstIterator {this->ptr, this->v};	
		}

		iterator& operator++()
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				return *this;
			this->ptr++;
			return *this;
		}

		iterator operator++(int)
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				return *this;
			return iterator {this->ptr++, this->v};
		}

		size_type operator-(const const_iterator& it) const
		{
			return static_cast<ConstIterator>(*this)-it;
		}
	};


	class ConstIterator 
	{
		public:
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
		using const_iterator = Vector::const_iterator;
		using size_type = Vector::size_type;
		using iterator = Vector::iterator;
		using v_pointer = const Vector*;


		private:
		pointer ptr;
		v_pointer v;


		public:
		ConstIterator()
		{
			this->ptr = nullptr;
			this->v = nullptr;
		}

		ConstIterator(pointer ptr, v_pointer v = nullptr)
		{
			this->ptr=ptr;
			this->v=v;
		}

		reference operator*() const
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				throw runtime_error {"1"};
			return *this->ptr;
		}
		pointer operator->() const
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				throw runtime_error {"1"};
			return this->ptr;
		}

		bool operator==(const const_iterator& it) const
		{
			return (this->ptr == it.ptr);
		}

		bool operator!=(const const_iterator& it) const
		{
			return (!(*this==it));
		}

		const_iterator& operator++()
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				return *this;
			this->ptr++;
			return *this;
		}

		const_iterator operator++(int)
		{
			if(this->v==nullptr || this->ptr==nullptr || this->ptr < this->v->values || this->ptr >= this->v->values+this->v->sz)
				return *this;
			return const_iterator {this->ptr++, this->v};
		}

		size_type operator-(const const_iterator& it) const
		{
			return static_cast<size_type>(this->ptr - it.ptr);
		}
	};
};

#endif