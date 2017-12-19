#pragma once
#ifndef GENERIC_H_INCLUDED
#define GENERIC_H_INCLUDED

#include <z/float.h>
#include <z/int.h>
#include <z/char.h>

#include <z/math/Operator.h>

#include "string.h"
#include "array.h"

#include <complex>

namespace z
{
    namespace core
    {
        enum type
        {
            NONE = 0,

            STRING,
            ARRAY,

            COMPLEX_FLOAT,
            COMPLEX_INT,

            FLOATING,
            INTEGER,

            TYPE_COUNT
        };

        class generic
        {
        private:
            bool _error;

            type _type;

            union
            {
                core::string<Char>* String;
                core::array<generic>* Array;

                std::complex<Float>* ComplexFloat;
                std::complex<Int>* ComplexInt;

                Float Floating;
                Int Integer;
            } data;

            void alloc(const generic&);
            void dealloc();

            bool comparableTo(const generic&) const;
            void operateOnMe(const generic&, math::Operator*) const;

        public:
            generic();
            generic(const generic&);
            generic(generic&&);
            generic(const core::string<Char>&);
            generic(const core::array<generic>&);
            generic(const std::complex<Float>&);
            generic(const std::complex<Int>&);
            generic(const Float&);
            generic(const Int&);

            ~generic();


            const generic& operator=(const generic&);

            const bool operator==(const generic&) const;
            const bool operator>(const generic&) const;
            const bool operator<(const generic&) const;

            inline const bool operator!=(const generic& other) const
            { return !operator==(other); }

            inline const bool operator>=(const generic& other) const
            { return !operator<(other); }

            inline const bool operator<=(const generic& other) const
            { return !operator>(other); }
/*
            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            generic(const std::complex<T>& _complex)
            {
                d_type = data::VALUE;
                d_value = new std::complex<Float>(_complex);
            }

            template <typename CHAR_2>
            generic(const core::string<CHAR_2>& _string)
            {
                d_type = data::STRING;
                d_string = new core::string<CHAR>(_string);
            }

            generic(const core::array< generic<CHAR> >&);

            template <typename CHAR_2>
            generic(const CHAR_2* cstring)
            {
                d_type = data::STRING;
                d_string = new core::string<CHAR>(cstring);
            }


            ~generic() { clear(); }

            void clear()
            {
                if (d_type == data::ERROR)
                    delete d_error;
                else if (d_type == data::VALUE)
                    delete d_value;
                else if (d_type == data::STRING)
                    delete d_string;
                else if (d_type == data::ARRAY)
                    delete d_array;

                d_type = data::NONE;
            }







            inline const data_t type() const {return d_type;}


            const core::string<CHAR> string() const;
            const std::complex<Float> complex() const;
            const Float real() const;
            const Float imag() const;

            inline core::array< generic<CHAR> >& array()
            { return *d_array; }

            inline const core::array< generic<CHAR> >& array() const
            { return *d_array; }

            const error err() const
            {
                if (d_type == data::ERROR)
                    return error(*d_error);
                else
                    return error();
            }*/


            const generic index(const generic&) const;
            const generic index(const generic&,
                               const generic&) const;

            const generic subIndex(const generic&) const;
            const generic subIndex(const generic&,
                                  const generic&) const;

            const generic& merge(const core::array< generic >&);

            //operators
            const generic& operator++();
            const generic& operator--();
            const generic operator++(int);
            const generic operator--(int);

            const generic operator-() const;
            const generic operator+(const generic&) const;
            const generic operator-(const generic&) const;

            const generic operator*(const generic&) const;
            const generic operator/(const generic&) const;
            const generic operator%(const generic&) const;
            const generic int_divide(const generic&) const;

            const generic operator^(const generic&) const;
            const generic factorial() const;

            const generic operator&(const generic&) const;
            const generic operator&&(const generic&) const;
            const generic operator|(const generic&) const;
            const generic operator||(const generic&) const;
            const generic operator!() const;
            const generic operator~() const;
            const generic xor_bitwise(const generic&) const;
            const generic xor_logical(const generic&) const;
            const generic nand_bitwise(const generic&) const;
            const generic nand_logical(const generic&) const;
            const generic nor_bitwise(const generic&) const;
            const generic nor_logical(const generic&) const;
            const generic nxor_bitwise(const generic&) const;
            const generic nxor_logical(const generic&) const;
        };

        generic::generic()
        {
            _type = type::NONE;
        }

        generic::generic(const generic& other)
        {
            alloc(other);
        }

        generic::generic(generic&& other)
        {
            _type = other._type;
            data = other.data;
        }

        generic::generic(const core::string<Char>& init)
        {
            _type = type::STRING;
            data.String = new core::string<Char>(init);
        }

        generic::generic(const core::array<generic>& init)
        {
            _type = type::ARRAY;
            data.Array = new core::array<generic>(init);
        }

        generic::generic(const std::complex<Float>& init)
        {
            _type = type::COMPLEX_FLOAT;
            data.ComplexFloat = new std::complex<Float>(init);
        }

        generic::generic(const std::complex<Int>& init)
        {
            _type = type::COMPLEX_INT;
            data.ComplexInt = new std::complex<Int>(init);
        }

        generic::generic(const Float& init)
        {
            _type = type::FLOATING;
            data.Floating = init;
        }

        generic::generic(const Int& init)
        {
            _type = type::INTEGER;
            data.Integer = init;
        }

        generic::~generic()
        {
            dealloc();
        }

        void generic::alloc(const generic& other)
        {
            _type = other._type;

            if (_type == type::STRING)
            {
                data.String =
                new core::string<Char>(*(other.data.String));
            }
            else if (_type == type::ARRAY)
            {
                data.Array =
                new core::array<generic>(*(other.data.Array));
            }
            else if (_type == type::COMPLEX_FLOAT)
            {
                data.ComplexFloat =
                new std::complex<Float>(*(other.data.ComplexFloat));
            }
            else if (_type == type::COMPLEX_INT)
            {
                data.ComplexInt =
                new std::complex<Int>(*(other.data.ComplexInt));
            }
            else
            {
                data = other.data;
            }
        }

        void generic::dealloc()
        {
            if (_type == type::STRING)
            {
                delete data.String;
            }
            else if (_type == type::ARRAY)
            {
                delete data.Array;
            }
            else if (_type == type::COMPLEX_FLOAT)
            {
                delete data.ComplexFloat;
            }
            else if (_type == type::COMPLEX_INT)
            {
                delete data.ComplexInt;
            }
        }

        bool generic::comparableTo(const generic& other) const
        {
            if (_type == other._type)
                return true;

            if ((_type > type::ARRAY) &&
                (other._type > type::ARRAY))
                return true;

            return false;
        }

        void generic::operateOnMe(const generic& other,
                                  math::Operator* oper) const
        {
            if (_type == other._type)
            {
                if (_type == type::COMPLEX_FLOAT)
                {
                    *(data.ComplexFloat) =
                        oper->operate(*(data.ComplexFloat),
                                      *(other.data.ComplexFloat));
                }
                else if (_type == type::COMPLEX_INT)
                {
                    *(data.ComplexInt) =
                        oper->operate(*(data.ComplexInt),
                                      *(other.data.ComplexInt));
                }
                else if (_type == type::FLOATING)
                {
                    data.Floating =
                        oper->operate(data.Floating,
                                      other.data.Floating);
                }
                else //_type == type::INTEGER
                {
                    data.Integer =
                        oper->operate(data.Integer,
                                      other.data.Integer);
                }
            }
            else
            {
                if (_type == type::COMPLEX_FLOAT)
                {
                    std::complex<Float> operand;
                    std::complex<Float> result;

                    if (other._type == type::COMPLEX_FLOAT)
                    {
                        operand = *(other.data.ComplexFloat);
                    }
                    else if (other._type == type::COMPLEX_INT)
                    {
                        operand = *(other.data.ComplexInt);
                    }
                    else if (other._type == type::FLOATING)
                    {
                        operand = *(other.data.Floating);
                    }
                    else //INTEGER
                    {
                        operand = *(other.data.Integer);
                    }

                    *(data.ComplexFloat) =
                    oper.operate(*(data.ComplexFloat), operand);
                }
                else if (_type == type::COMPLEX_INT)
                {

                }
                else if (_type == type::FLOATING)
                {

                }
                else if (_type == type::INTEGER)
                {

                }
            }

            _error = oper.error();
            delete oper;
        }

        const generic& generic::operator=(const generic& other)
        {
            dealloc();

            alloc(other);

            return *this;
        }

        const bool generic::operator==(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) == *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) == *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return true;
                }
                else //numeric type
                {
                    int result = compareTo(other);

                    return (result == 0);
                }
            }

            return false;
        }

/*
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::index(const generic<CHAR>& _index) const
        {
            generic<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type <= data::VALUE)
            {
                result = error("Cannot index");
            }
            else if (d_type == data::ARRAY)
            {
                int i_index = (int)_index.d_value->real();

                if (d_array->is_valid(i_index))
                    result = d_array->at(i_index);
                else
                {
                    result = error("Index out of bounds");
                }
            }
            else //STRING and (_index.type = VALUE)
            {
                int i_index = (int)_index.d_value->real();

                result = core::string<CHAR>(d_string->at(i_index));
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::index(const generic<CHAR>& start,
                                            const generic<CHAR>& stop) const
        {
            generic<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value->imag() != (Float)0) ||
                (stop.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type <= data::VALUE)
            {
                result = error("Cannot index");
            }
            else if (d_type == data::ARRAY)
            {
                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                if (d_array->is_valid(i_start) &&
                    d_array->is_valid(i_stop))
                    result = d_array->subset(i_start, i_stop);
                else
                {
                    result = error("Index out of bounds");
                }
            }
            else //STRING
            {
                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                result = d_string->substr(i_start, i_stop);
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::subIndex(const generic<CHAR>& _index) const
        {
            generic<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type != data::ARRAY)
            {
                result = error("Cannot index");
            }
            else
            {
                result.d_type = data::ARRAY;

                int i_index = (int)_index.d_value->real();

                for (int i=0; i<d_array->size(); i++)
                {
                    if (d_array->at(i).d_type <= data::VALUE)
                    {
                        result = error("Cannot index");
                        return result;
                    }
                    else if (d_array->at(i).d_type == data::ARRAY)
                    {
                        if (d_array->at(i).d_array->is_valid(i_index))
                        {
                            result.d_array.add(d_array->at(i).
                                               d_array->at(i_index));
                        }
                        else
                        {
                            result = error("Index out of bounds");
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(core::string<char>(
                                        d_array->at(i).
                                        d_string->at(i_index)));
                    }
                }
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::subIndex(const generic<CHAR>& start,
                                                  const generic<CHAR>& stop) const
        {
            generic<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value->imag() != (Float)0) ||
                (stop.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type != data::ARRAY)
            {
                result = error("Cannot index");
            }
            else
            {
                result = data::ARRAY;

                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                for (int i=0; i<d_array.size(); i++)
                {
                    if (d_array->at(i).d_type <= data::VALUE)
                    {
                        result = error("Cannot index");
                        return result;
                    }
                    else if (d_array->at(i).d_type == data::ARRAY)
                    {
                        if (d_array->at(i).d_array->is_valid(i_start) &&
                            d_array->at(i).d_array->is_valid(i_stop))
                        {

                            result.d_array.add(
                                generic<CHAR>(d_array->at(i).
                                d_array->subset(i_start, i_stop)));
                        }
                        else
                        {
                            result = error("Index out of bounds");
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(d_array->at(i).
                                           d_string->substr(i_start, i_stop));
                    }
                }
            }

            return result;
        }


        ///Assuming a 2D array list, change array grouping
        ///from horizontal to vertical.
        /**

        {a,b,c}     {a,d,g}
        {d,e,f} --> {b,e,h}
        {g,h,i}     {c,f,i}

        */
     /*   template <typename CHAR>
        const generic<CHAR>& generic<CHAR>::merge(const core::array< generic<CHAR> >& arr)
        {
            clear();
            d_type = data::ARRAY;


            int max_width = 1;

            //find max array width
            for (int i=0; i<arr.size(); i++)
            {
                if((arr[i].d_type == data::ARRAY) &&
                   (arr[i].d_array->size() > max_width))
                {
                    max_width = arr[i].d_array->size();
                }
            }


            //fill new arrays in order
            for (int i=0; i<max_width; i++)
            {
                core::array< generic<CHAR> > current;

                for (int j=0; j<arr.size(); j++)
                {
                    int width;
                    if (arr[j].d_type == data::ARRAY)
                        width = arr[j].d_array->size();
                    else
                        width = 1;


                    if (width > i)
                    {
                        if (arr[j].d_type == data::ARRAY)
                            current.add(arr[j].d_array->at(i));
                        else
                            current.add(arr[j]);
                    }
                }

                d_array->add(generic<CHAR>(current));
            }


            return *this;
        }


        template <typename CHAR>
        const generic<CHAR>& generic<CHAR>::operator++()
        {
            if (d_type == data::VALUE)
            {
                *d_value = *d_value + (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                delete d_array;

                d_type = data::ERROR;
                d_error = new error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                delete d_string;

                d_type = data::ERROR;
                d_error = new error("Invalid operation on string");
            }

            return *this;
        }

        template <typename CHAR>
        const generic<CHAR>& generic<CHAR>::operator--()
        {
            if (d_type == data::VALUE)
            {
                *d_value = *d_value - (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                delete d_array;

                d_type = data::ERROR;
                d_error = new error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                delete d_string;

                d_type = data::ERROR;
                *d_error = new error("Invalid operation on string");
            }

            return *this;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator++(int)
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
            {
                result = *this;
                *d_value = *d_value + (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }

            return result;
        }

        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator--(int)
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
            {
                result = *this;
                *d_value = *d_value - (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        template <typename CHAR>
        const bool generic<CHAR>::operator==(const generic<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return false;

            switch (d_type)
            {
            case (data::VALUE):
                return *d_value == *other.d_value;
                break;

            case (data::STRING):
                return *d_string == *other.d_string;
                break;

            case (data::ARRAY):
                return *d_array == *other.d_array;
                break;

            default:
                return true;
            }
        }

        template <typename CHAR>
        const bool generic<CHAR>::operator>(const generic<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return (d_type > other.d_type);

            switch (d_type)
            {
            case (data::VALUE):
                return *d_value > *other.d_value;
                break;

            case (data::STRING):
                return *d_string > *other.d_string;
                break;

            case (data::ARRAY):
                return *d_array > *other.d_array;
                break;

            default:
                return true;
            }
        }

        template <typename CHAR>
        const bool generic<CHAR>::operator<(const generic<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return (d_type < other.d_type);

            switch (d_type)
            {
            case (data::VALUE):
                return *d_value < *other.d_value;
                break;

            case (data::STRING):
                return *d_string < *other.d_string;
                break;

            case (data::ARRAY):
                return *d_array < *other.d_array;
                break;

            default:
                return true;
            }
        }


        template <typename CHAR>
        const generic<CHAR>& generic<CHAR>::operator=(const generic<CHAR>& other)
        {
            d_type = other.d_type;

            if (d_type == data::VALUE)
                d_value = new std::complex<Float>(*other.d_value);
            else if (d_type == data::STRING)
                d_string = new core::string<CHAR>(*other.d_string);
            else if (d_type == data::ARRAY)
                d_array = new core::array< generic<CHAR> >(*other.d_array);
            else if (d_type == data::ERROR)
                d_error = new error(*other.d_error);

            return *this;
        }


        template <typename CHAR>
        generic<CHAR>::generic(const generic<CHAR>& other)
        {
            d_type = other.d_type;

            if (d_type == data::VALUE)
                d_value = new std::complex<Float>(*other.d_value);
            else if (d_type == data::STRING)
                d_string = new core::string<CHAR>(*other.d_string);
            else if (d_type == data::ARRAY)
                d_array = new core::array< generic<CHAR> >(*other.d_array);
            else if (d_type == data::ERROR)
                d_error = new error(*other.d_error);
        }

        template <typename CHAR>
        generic<CHAR>::generic(generic<CHAR>&& other)
        {
            d_type = other.d_type;

            d_value = other.d_value;

            other.d_type = data::NONE;
        }


        template <typename CHAR>
        generic<CHAR>::generic(const core::array< generic<CHAR> >& _array)
        {
            d_type = data::ARRAY;
            d_array = new core::array< generic<CHAR> >(_array);
        }



        template <typename CHAR>
        const core::string<CHAR> generic<CHAR>::string() const
        {
            if (d_type == data::VALUE)
                return core::string<CHAR>(*d_value);
            else if (d_type == data::STRING)
                return *d_string;
            else if (d_type == data::ARRAY)
            {
                core::string<CHAR> r_string = "{";

                for (int i=0; i<d_array->size(); i++)
                {
                    if (i > 0)
                        r_string += ",";

                    r_string += d_array->at(i).string();
                }

                r_string += "}";

                return r_string;
            }
            else if (d_type == data::ERROR)
            {
                return core::string<CHAR>("[")+
                        d_error->message()+
                        core::string<CHAR>("]");
            }
            else
                return core::string<CHAR>("NUL");

        }


        template <typename CHAR>
        const std::complex<Float> generic<CHAR>::complex() const
        {
            if (d_type == data::VALUE)
                return *d_value;
            else if (d_type == data::STRING)
                return d_string->complexValue();
            else
                return std::complex<Float>();
        }


        template <typename CHAR>
        const Float generic<CHAR>::real() const
        {
            if (d_type == data::VALUE)
                return d_value->real();
            else if (d_type == data::STRING)
                return d_string->complexValue().real();
            else
                return 0;
        }

        template <typename CHAR>
        const Float generic<CHAR>::imag() const
        {
            if (d_type == data::VALUE)
                return d_value->imag();
            else if (d_type == data::STRING)
                return d_string->complexValue().imag();
            else
                return 0;
        }




        ///Operators

        //negation
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator-() const
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
                result = -(*d_value);
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }

            return result;
        }

        //addition
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator+(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                if (d_type == data::STRING)
                    result = *d_string;
                else
                    result = this->string();

                if (other.d_type == data::STRING)
                    *(result.d_string) += *(other.d_string);
                else
                    *(result.d_string) += other.string();
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = *d_value + *other.d_value;
            }

            return result;
        }

        //subtraction
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator-(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (*d_value) - (*other.d_value);
            }

            return result;
        }

        //multiplication
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator*(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (*d_value) * (*other.d_value);
            }

            return result;
        }

        //floating-point division
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator/(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value->real() || other.d_value->imag())
                    result = *d_value / *other.d_value;
                else
                {
                    result = error("Division by zero");
                }

            }

            return result;
        }

        //remainder of integer division
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator%(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value->real() || other.d_value->imag())
                {
                    result = math::remainder(*d_value, *(other.d_value));
                }
                else
                {
                    result = error("Division by zero");
                }

            }

            return result;
        }

        //integer division
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::int_divide(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value->real() || other.d_value->imag())
                {
                    result = std::complex<long>(*d_value) /
                             std::complex<long>(*other.d_value);
                }
                else
                {
                    result = error("Division by zero");
                }

            }

            return result;
        }

        //power
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator^(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = pow(*d_value,*(other.d_value));
            }

            return result;
        }

        //factorial
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::factorial() const
        {
            generic<CHAR> result;

            if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::VALUE)
            {
                if (d_value->imag() != 0)
                {
                    result = error("Factorial requires a real number");
                }
                else if (d_value->real() < 0)
                {
                    result = error("Factorial must be non-negative");
                }
                else
                {
                    Float _rslt = math::factorial((long)d_value->real());

                    if (_rslt == 0)
                    {
                        result = error("Factorial input overflow");
                    }
                    else
                        result = _rslt;
                }
            }

            return result;
        }

        //logical AND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator&&(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() && (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise AND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator&(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() & (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical OR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator||(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() || (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise OR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator|(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() | (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NOT
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator!() const
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
                result = !(long)d_value->real();
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }

            return result;
        }


        //bitwise NOT
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator~() const
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
                result = ~(long)d_value->real();
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }

            return result;
        }

        //logical XOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::xor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value->real() != !(long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise XOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::xor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() ^ (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NAND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nand_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value->real() && (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NAND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nand_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() & (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value->real() || (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() | (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NXOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nxor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value->real() == !(long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NXOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nxor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() ^ (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }*/

        /*class generic
        {
        protected:
            bool _error;

        public:
            generic() {_error = false;}
            virtual ~generic() {}

            inline bool error()
            {
                return _error;
            }

            //equality methods
            virtual const bool operator==(const generic&) const;
            virtual const bool operator!=(const generic&) const;
            virtual const bool operator>(const generic&) const;
            virtual const bool operator<(const generic&) const;
            virtual const bool operator>=(const generic&) const;
            virtual const bool operator<=(const generic&) const;

            //casting methods
            virtual const core::string<Char> string() const;
            virtual const core::array<generic>* array() const;
            virtual const std::complex<Float> complexFloat() const;
            virtual const std::complex<Int> complexInt() const;
            virtual const Float floating() const;
            virtual const Int integer() const;

            //type checking
            virtual bool isNull() const;
            virtual bool isString() const;
            virtual bool isArray() const;
            virtual bool isComplexFloat() const;
            virtual bool isComplexInt() const;
            virtual bool isFloating() const;
            virtual bool isInteger() const;

            //operators
            const generic& operator++();
            const generic& operator--();
            const generic operator++(int);
            const generic operator--(int);

            const generic operator-() const;
            const generic operator+(const generic&) const;
            const generic operator+=(const generic&) const;
            const generic operator-(const generic&) const;
            const generic operator-=(const generic&) const;

            const generic operator*(const generic&) const;
            const generic operator/(const generic&) const;
            const generic operator%(const generic&) const;
            const generic int_divide(const generic&) const;
            const generic int_divide_equals(const generic&) const;

            const generic operator^(const generic&) const;
            const generic factorial() const;

            const generic operator&(const generic&) const;
            const generic operator&&(const generic&) const;
            const generic operator|(const generic&) const;
            const generic operator||(const generic&) const;
            const generic operator!() const;
            const generic operator~() const;
            const generic xor_bitwise(const generic&) const;
            const generic xor_logical(const generic&) const;
            const generic nand_bitwise(const generic&) const;
            const generic nand_logical(const generic&) const;
            const generic nor_bitwise(const generic&) const;
            const generic nor_logical(const generic&) const;
            const generic nxor_bitwise(const generic&) const;
            const generic nxor_logical(const generic&) const;
        };

        const core::string<Char> generic::string() const
        {
            return core::string<Char>();
        }

        const core::array<generic>* generic::array() const
        {
            return NULL;
        }

        const std::complex<Float> generic::complexFloat() const
        {
            return std::complex<Float>();
        }

        const std::complex<Int> generic::complexInt() const
        {
            return std::complex<Int>();
        }

        const Float generic::floating() const
        {
            return 0;
        }

        const Int generic::integer() const
        {
            return 0;
        }*/
    }
}

#endif // GENERIC_H_INCLUDED
