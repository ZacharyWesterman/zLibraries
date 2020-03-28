#pragma once

#include <vector>
#include "stream.hpp"
#include "sizable.hpp"
#include "serializable.hpp"
#include "typeChecks.hpp"
#include "compare.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief A wrapper for std::vector.
		 *
		 * This class is a wrapper for the std::vector class
		 * that adds ease of use and hides implementation from the user.
		 *
		 * /note This will only compile for objects that are copyable.
		 * Use std::vector for non-copyable objects.
		 *
		 * \see refArray
		 * \see sortedArray
		 * \see sortedRefArray
		 */
		template <typename T>
		class array : public sizable, public serializable
		{
		protected:
			///The data in the array.
			std::vector<T> array_data;

			/**
			 * \brief Helper function for single object initialization.
			 * \param arg1 The object to initialize the array with.
			 */
			inline void init(const T& arg1)
			{
				add(arg1);
			}

			/**
			 * \brief Helper function for brace-enclosed list initialization.
			 * \param arg1 The first object to add to the array.
			 * \param args Any following objects to add to the array.
			 */
			template <typename... Args>
			inline void init(const T& arg1, const Args&... args)
			{
				add(arg1);

				init(args...);
			}

			/**
			 * \brief Check if two objects are equal.
			 *
			 * This member function allows object comparison to be
			 * different for different array types (e.g. reference arrays
			 * will need to sort by value after dereference).
			 *
			 * \param arg1 First object to compare.
			 * \param arg2 Second object to compare.
			 *
			 * \return True if the objects are equal, false otherwise.
			 */
			virtual bool eq(const T& arg1, const T& arg2) const
			{
				return equals(arg1, arg2);
			}

			/**
			 * \brief Check if one object is greater than another.
			 *
			 * This member function allows object comparison to be
			 * different for different array types (e.g. reference arrays
			 * will need to sort by value after dereference).
			 *
			 * \param arg1 First object to compare.
			 * \param arg2 Second object to compare.
			 *
			 * \return True if arg1 is greater than arg2, false otherwise.
			 */
			virtual bool gt(const T& arg1, const T& arg2) const
			{
				return greater(arg1, arg2);
			}

			/**
			 * \brief Check if one object is less than another.
			 *
			 * This member function allows object comparison to be
			 * different for different array types (e.g. reference arrays
			 * will need to sort by value after dereference).
			 *
			 * \param arg1 First object to compare.
			 * \param arg2 Second object to compare.
			 *
			 * \return True if arg1 is less than arg2, false otherwise.
			 */
			virtual bool lt(const T& arg1, const T& arg2) const
			{
				return lesser(arg1, arg2);
			}

		public:
			///Default constructor.
			array() {}

			array(const array&);

			template <typename... Args>
			array(const T& arg1, const Args&... args);

			///Destructor
			virtual ~array() {}

			inline void clear();

			/**
			 * \brief Add an object to the array.
			 *
			 * Adds the given data to a position in the array. That
			 * position is up to implementation.
			 *
			 * \param object the data to add to the array.
			 *
			 * \return The index where the inserted object now resides.
			 */
			virtual size_t add(const T& object)
			{
				array_data.push_back(object);

				return (array_data.size() - 1);
			}

			/**
			 * \brief Add another array to this array.
			 *
			 * Copies the contents of another array and
			 * appends them to the end of this array.
			 *
			 * \param other the array to copy from.
			 */
			void add(const array& other)
			{
				for (size_t i=0; i<other.size(); i++)
					add(other.array_data[i]);
			}

			array& insert(const T&, size_t);

			void append(const T&);

			array& remove(size_t);
			array& remove(size_t, int);

			array& replace(size_t, int, const T&);
			array& replace(size_t, int, const array<T>&);


			array subset(size_t, int) const;

			size_t size() const;

			size_t length() const;

			T& at(size_t);
			const T& at(size_t) const;
			T& operator[](size_t);
			const T& operator[](size_t) const;

			/**
			 * \brief Check if a given object is in the array.
			 *
			 * Locates the desired index using a linear search,
			 * as the array is expected to be unsorted.
			 *
			 * \param object the object to search for.
			 *
			 * \return The first index that the object was found at.
			 * \b -1 if it was not found.
			 */
			virtual intmax_t find(const T& object) const
			{
				for (size_t i=0; i<array_data.size(); i++)
					if (eq(array_data.at(i),object))
						return i;

				return -1;
			}

			array& operator=(const array& other);

			bool operator==(const array& other) const;
			bool operator>(const array& other) const;
			bool operator<(const array& other) const;
			inline bool operator>=(const array& other) const;
			inline bool operator<=(const array& other) const;


			bool isValid(size_t position) const;

			void serialIn(inputStream&);
			void serialOut(outputStream&) const;

			/**
			 * \brief Get pointer to the beginning of the array.
			 *
			 * This member function should not be used directly.
			 * It is meant for C++11's range-based for loop syntax.
			 *
			 * \return A pointer to the first element in the array. 0 if no elements.
			 */
			T* begin() const
			{
				return array_data.empty() ? NULL : (T*)&array_data.front();
			}

			/**
			 * \brief Get pointer to the end of the array.
			 *
			 * This member function should not be used directly.
			 * It is meant for C++11's range-based for loop syntax.
			 *
			 * \return A pointer to right after the last element in the array. 0 if no elements.
			 */
			T* end() const
			{
				return begin() + array_data.size();
			}
		};


		///Copy constructor
		template <typename T>
		array<T>::array(const array<T>& other)
		{
			array_data = other.array_data;
		}

		/**
		 * \brief List-initialized constructor.
		 *
		 * Constructs the array with an arbitrary
		 * number of elements already contained.<BR>
		 *
		 * \b Syntax: array<T> X {arg1, arg2, ...};
		 * array<T> X = {arg1, arg2, ...};
		 *
		 * \param arg1 initializing data.
		 * \param args cont. initializing data.
		 */
		template <typename T>
		template <typename... Args>
		array<T>::array(const T& arg1, const Args&... args)
		{
			init(arg1, args...);
		}

		/**
		 * \brief Array assignment operator.
		 *
		 * Clear the contents of this array and create
		 * a copy of another array's contents into this one.
		 *
		 * \param other the array to copy from.
		 *
		 * \return This array after the operation (for
		 * \b a=b=c type expressions).
		 */
		template <typename T>
		array<T>& array<T>::operator=(const array<T>& other)
		{
			array_data = other.array_data;

			return *this;
		}

		/**
		 * \brief Check whether two arrays' contents are the same.
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if both arrays contain the same
		 * contents in the same order, and the same number of
		 * contents. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator==(const array<T>& other) const
		{
			if (array_data.size() != other.array_data.size())
				return false;

			for (size_t i=0; i<array_data.size(); i++)
				if (!eq(array_data.at(i),other.array_data.at(i)))
					return false;

			return true;
		}

		/**
		 * \brief Array greater-than operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if the difference between the two arrays'
		 * elements add to a positive number, or this array has more
		 * elements. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator>(const array<T>& other) const
		{
			if (array_data.size() != other.array_data.size())
				return (array_data.size() > other.array_data.size());

			int gt_count = 0;

			for (size_t i=0; i<array_data.size(); i++)
			{
				if (gt(array_data.at(i),other.array_data.at(i)))
					gt_count++;
				else if (lt(array_data.at(i),other.array_data.at(i)))
					gt_count--;
			}

			return gt_count > 0;
		}

		/**
		 * \brief Array less-than operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if the difference between the two arrays'
		 * elements add to a negative number, or this array has fewer
		 * elements. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator<(const array& other) const
		{
			if (array_data.size() != other.array_data.size())
				return (array_data.size() < other.array_data.size());

			int gt_count = 0;

			for (size_t i=0; i<array_data.size(); i++)
			{
				if (gt(array_data.at(i),other.array_data.at(i)))
					gt_count++;
				else if (lt(array_data.at(i),other.array_data.at(i)))
					gt_count--;
			}

			return gt_count < 0;
		}

		/**
		 * \brief Array greater-than-or-equal operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b False if the difference between the two arrays'
		 * elements add to a negative number, or this array has fewer
		 * elements. \b True otherwise.
		 */
		template <typename T>
		inline bool array<T>::operator>=(const array& other) const
		{
			return !operator<(other);
		}

		/**
		 * \brief Array less-than-or-equal operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b False if the difference between the two arrays'
		 * elements add to a positive number, or this array has more
		 * elements. \b True otherwise.
		 */
		template <typename T>
		inline bool array<T>::operator<=(const array<T>& other) const
		{
			return !operator>(other);
		}


		///Clear the data in the array.
		template <typename T>
		inline void array<T>::clear()
		{
			array_data.clear();
		}

		/**
		 * \brief Insert an object into the array.
		 *
		 * Inserts an object into the given index in the array, if possible.
		 *
		 * \param object the data to add to the array.
		 * \param index the index in the array to insert the object.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		array<T>& array<T>::insert(const T& object, size_t index)
		{
			//if invalid index, return false
			if (index > array_data.size())
				return *this;

			array_data.insert(array_data.begin() + index, object);

			return *this;
		}

		/**
		 * \brief Append an object to the end of the array.
		 *
		 * Appends the given data to the end of the array.
		 *
		 * \param object the data to aappend to the array.
		 */
		template <typename T>
		void array<T>::append(const T& object)
		{
			array_data.push_back(object);
		}

		/**
		 * \brief Remove an object from the array.
		 *
		 * \param index the index of the object to be removed.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		array<T>& array<T>::remove(size_t index)
		{
			if (index >= array_data.size())
				return *this;

			array_data.erase(array_data.begin() + index);

			return *this;
		}

		/**
		 * \brief Remove all elements in a subset of the array.
		 *
		 * \param index the index of the first object to be removed.
		 * \param count the number of objects to be removed.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		array<T>& array<T>::remove(size_t index, int count)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return *this;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);

			return *this;
		}


		/**
		 * \brief Get the size of the array.
		 *
		 * \return The (approximate) number of bytes the array consumes.
		 */
		template <typename T>
		size_t array<T>::size() const
		{
			size_t bytes = 0;
			for (size_t i=0; i<array_data.size(); i++)
			{
				size_t objBytes;
				z::core::size(array_data[i], objBytes);
				bytes += objBytes;
			}
			return bytes;
		}

		/**
		 * \brief Get the length of the array.
		 *
		 * \return The number of objects in the array.
		 */
		template <typename T>
		size_t array<T>::length() const
		{
			return array_data.size();
		}

		/**
		 * \brief Function to get the object at the given index.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see operator[](size_t)
		 */
		template <typename T>
		T& array<T>::at(size_t index)
		{
			return array_data.at(index);
		}

		/**
		 * \brief Const function to get the object at the given index.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see operator[](size_t) const
		 */
		template <typename T>
		const T& array<T>::at(size_t index) const
		{
			return array_data.at(index);
		}

		/**
		 * \brief Function to get the object at the given index.
		 *
		 * Identical behavior to at(size_t), but allows indexing
		 * with square brackets.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see at(size_t)
		 */
		template <typename T>
		T& array<T>::operator[](size_t index)
		{
			return array_data.at(index);
		}

		/**
		 * \brief Const function to get the object at the given index.
		 *
		 * Identical behavior to at(size_t), but allows indexing
		 * with square brackets.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see at(size_t) const
		 */
		template <typename T>
		const T& array<T>::operator[](size_t index) const
		{
			return array_data.at(index);
		}


		/**
		 * \brief Replace all objects in the given range with an object.
		 *
		 * \param index the index of the first object to replace.
		 * \param count the number of objects to replace.
		 * \param object the object to insert into the gap.
		 *
		 * \return A reference to this array after modification.
		 *
		 * \see replace(size_t,int,const array&)
		 */
		template <typename T>
		array<T>& array<T>::replace(size_t index, int count, const T& object)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return *this;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);
			array_data.insert(array_data.begin() + start, object);

			return *this;
		}

		/**
		 * \brief Replace all objects in the given range with an array of objects.
		 *
		 * \param index the index of the first object to replace.
		 * \param count the number of objects to replace.
		 * \param other the array to copy from.
		 *
		 * \return A reference to this array after modification.
		 *
		 * \see replace(size_t,int,const T&)
		 */
		template <typename T>
		array<T>& array<T>::replace(size_t index, int count, const array<T>& other)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return *this;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);

			for (size_t i=0; i<other.size(); i++)
				array_data.insert(array_data.begin() + start + i, other[i]);

			return *this;
		}


		/**
		 * \brief Get a contiguous subset of the elements in the array.
		 *
		 * Copies all elements in the given range, inclusive. If either
		 * of the parameters is \b -1, gives an empty array. If the
		 * \b stop parameter is less than \b start, then the subset is
		 * copied in reverse order.
		 *
		 * \param index the index of the first object to copy.
		 * \param count the number of objects to copy.
		 *
		 * \return A subset of the main array.
		 */
		template <typename T>
		array<T> array<T>::subset(size_t index, int count) const
		{
			array<T> output;

			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index;

				size_t offset = end+start;
				for (size_t i=start; i<end; i++)
					output.array_data.push_back(array_data[offset-i]);
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;

				for (size_t i=start; i<end; i++)
					output.array_data.push_back(array_data[i]);
			}

			return output;
		}


		/**
		 * \brief Check if an index is within the bounds of the array.
		 *
		 * \param index the index to check.
		 *
		 * \return \b True if the given index is within array bounds.
		 * \b False otherwise.
		 */
		template <typename T>
		bool array<T>::isValid(size_t index) const
		{
			return (index < array_data.size());
		}

		template <typename T>
		void array<T>::serialIn(inputStream& stream)
		{
			size_t count = 0;
			z::core::serialIn(count, stream);

			for (size_t i=0; i<count; i++)
			{
				T object{};
				z::core::serialIn(object, stream);
				array_data.push_back(std::move(object));
			}
		}

		template <typename T>
		void array<T>::serialOut(outputStream& stream) const
		{
			size_t count = array_data.size();
			z::core::serialOut(count, stream);

			for (size_t i=0; i<count; i++)
			{
				z::core::serialOut(array_data[i], stream);
			}
		}

	}
}