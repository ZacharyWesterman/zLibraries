#pragma once

namespace z
{
	namespace core
	{
		template <>
		void string<utf16>::increase(size_t goal)
		{
			if (data_len >= goal) return;

			uint8_t* old_data = data;
			size_t old_data_len = data_len;

			//~1.5x string growth
			while (data_len < goal)
				data_len += (data_len >> 1) + 4;
			data = new uint8_t[data_len];

			size_t remain = old_data_len;
			uint32_t* data32 = (uint32_t*)data;
			uint32_t* old32 = (uint32_t*)old_data;

			size_t i = 0;

			//copy as much data as possible in 32-bit chunks
			while (remain > 4)
			{
				data32[i] = old32[i];

				i++;
				remain -= sizeof(uint32_t);
			}

			//copy remaining amount
			while (remain)
			{
				size_t offset = old_data_len - remain;
				data[offset] = old_data[offset];

				remain--;
			}
		}

		template <>
		string<utf16>::string()
		{
			data = new uint8_t[2];
			*((uint16_t*)data) = 0;

			data_len = 2;
			character_ct = 0;
		}

		template <>
		string<utf16>::string(char chr)
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

			data16[0] = chr;
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(wchar_t chr)
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

			data16[0] = (chr > 0xFFFF) ? '?' : chr;
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(const uint32_t& chr)
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

			data16[0] = (chr > 0xFFFF) ? '?' : chr;
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(const char* str)
		{
			if (str)
			{
				size_t len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<1];
				data_len = len<<1;

				uint16_t* data16 = (uint16_t*)data;

				for (size_t i=0; i<len; i++)
					data16[i] = str[i];
			}
			else
			{
				data = new uint8_t[2];
				*((uint16_t*)data) = 0;

				data_len = 2;
				character_ct = 0;
			}
		}

		template <>
		string<utf16>::string(const wchar_t* str)
		{
			if (str)
			{
				size_t len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<1];
				data_len = len<<1;

				uint16_t* data16 = (uint16_t*)data;

				for (size_t i=0; i<len; i++)
				{
					if (str[i] > 0xFFFF)
						data16[i] = '?';
					else
						data16[i] = str[i];
				}
			}
			else
			{
				data = new uint8_t[2];
				*((uint16_t*)data) = 0;

				data_len = 2;
				character_ct = 0;
			}
		}

		template <>
		string<utf16>::string(const string<ascii>& other)
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			uint16_t* data16 = (uint16_t*)data;

			for (size_t i=0; i<=character_ct; i++)
				data16[i] = other.data[i];
		}

		template <>
		string<utf16>::string(const string<utf8>& other)
		{
			character_ct = 0;
			for (size_t i=0; i<other.character_ct; i+=lenFromUTF8(&other.data[i]))
				character_ct++;

			data_len = (character_ct + 1) << 1;
			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			data16[character_ct] = 0;

			size_t pos = 0;
			for (size_t i=0; i<=character_ct; i++)
			{
				uint32_t val = fromUTF8(&other.data[pos]);
				pos += lenFromUTF8(&other.data[pos]);

				data16[i] = (val > 0xFFFF) ? '?' : val;
			}
		}

		template <>
		string<utf16>::string(const string<utf16>& other)
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			for (size_t i=0; i<=character_ct; i++)
				data16[i] = other16[i];
		}

		template <>
		string<utf16>::string(const string<utf32>& other)
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			uint32_t* data32 = (uint32_t*)other.data;

			for (size_t i=0; i<=character_ct; i++)
				data16[i] = (data32[i] > 0xFFFF) ? '?' : data32[i];
		}

		template <>
		uint32_t string<utf16>::at(size_t index) const
		{
			uint16_t* data16 = (uint16_t*)data;

			if (index < character_ct)
				return data16[index];
			else
				return 0;
		}

		template <>
		const uint8_t* string<utf16>::cstring() const
		{
			return 0;
		}

		template <>
		const uint16_t* string<utf16>::nstring() const
		{
			return (uint16_t*)data;
		}

		template <>
		const uint32_t* string<utf16>::wstring() const
		{
			return 0;
		}

		template <>
		void string<utf16>::initChar(uint32_t chr, size_t index)
		{
			uint16_t* data16 = (uint16_t*)data;
			data16[index] = chr;
		}

		template <>
		size_t string<utf16>::charSize() const
		{
			return 2;
		}

		///operators
		template <>
		const string<utf16>& string<utf16>::operator+=(const string<utf16>& other)
		{
			size_t new_size = character_ct + other.character_ct + 1;
			this->increase(new_size << 1);

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			for (size_t i=0; i<other.character_ct; i++)
			{
				data16[character_ct + i] = other16[i];
			}
			data16[new_size-1] = 0;

			character_ct += other.character_ct;

			return *this;
		}

		///analyzers
		template <>
		int string<utf16>::count(const string<utf16>& other) const
		{
			if (!other.character_ct) return 0;

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			size_t occurrence = 0;

			size_t other_i = 0;
			for (size_t i=0; i<character_ct; i++)
			{
				if (data16[i] == other16[other_i])
				{
					other_i++;
					if (other_i >= other.character_ct)
						occurrence++;

					if (!occurrence) return (i - other_i + 1);
				}
				else
				{
					if ((character_ct - i) <= other.character_ct)
						return occurrence;

					other_i = 0;
				}
			}

			return occurrence;
		}

		template <>
		int string<utf16>::findAfter(const string<utf16>& other, size_t index, int occurrence) const
		{
			if (!other.character_ct || (occurrence < 1)) return -1;

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			size_t other_i = 0;
			for (size_t i=index; i<character_ct; i++)
			{
				//reset to first char of other if not still matching
				if (data16[i] != other16[other_i])
					other_i = 0;

				if (data16[i] == other16[other_i])
				{
					other_i++;
					if (other_i >= other.character_ct)
					{
						occurrence--;
						other_i = 0;
					}

					if (!occurrence) return (i - other.character_ct + 1);
				}
			}

			return -1;
		}

		template <>
		int string<utf16>::findBefore(const string<utf16>& other, size_t index, int occurrence) const
		{
			if (!other.character_ct || (occurrence < 1)) return -1;

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			if (index > (character_ct - other.character_ct))
				index = character_ct - other.character_ct;

			size_t other_i = other.character_ct - 1;
			for (size_t i=index; i<character_ct; i--)
			{
				if (data16[i] == other16[other_i])
				{

					if (!other_i)
					{
						occurrence--;

						if (occurrence && (i < other.character_ct)) i = 0;
						other_i = other.character_ct - 1;
					}
					else
						other_i--;

					if (!occurrence) return i;
				}
				else
				{
					if (occurrence && (i < other.character_ct)) return -1;
					other_i = other.character_ct - 1;
				}
			}

			return -1;
		}

		template <>
		bool string<utf16>::isInteger(int base) const
		{
			if ((base < 2) || (base > 36)) return false;

			uint16_t* data16 = (uint16_t*)data;

			if (!character_ct) return false;

			size_t start = ((data16[0] == '-') || (data16[0] == '+'));

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data16[i], base))
					return false;
			}

			return true;
		}

		template <>
		bool string<utf16>::isFloating(int base) const
		{
			if ((base < 2) || (base > 36)) return false;

			if (!character_ct) return false;

			uint16_t* data16 = (uint16_t*)data;

			bool pastDecimal, pastExponent;
			pastDecimal = pastExponent = false;

			size_t start = ((data16[0] == '-') || (data16[0] == '+'));

			if (start >= character_ct) return 0;

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data16[i], base))
				{
					if (data16[i] == '.')
					{
						if (pastDecimal || pastExponent)
							return false;
						else
						{
							if (i >= character_ct-1) return false;
							pastDecimal = true;
						}
					}
					else if (toLower(data16[i]) == 'e')
					{
						if (pastExponent)
							return false;
						else
						{
							pastExponent = true;
							if ((data16[i+1] == '+') || (data16[i+1] == '-'))
								i++;
						}
					}
					else return false;
				}
			}

			return true;
		}

		template <>
		bool string<utf16>::isComplex(int base) const
		{
			if ((base < 2) || (base > 36)) return false;

			if (!character_ct) return false;

			bool pastDecimal, pastExponent, imag, ir;
			pastDecimal = pastExponent = imag = ir = false;

			uint16_t* data16 = (uint16_t*)data;

			size_t start = ((data16[0] == '-') || (data16[0] == '+'));

			if (start >= character_ct) return 0;

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data16[i], 10))
				{
					if (data16[i] == '.')
					{
						if (pastDecimal || pastExponent)
							return false;
						else
						{
							if ((i >= character_ct-1) || (toLower(data16[i+1]) == 'i'))
								return false;
							pastDecimal = true;
						}
					}
					else if (toLower(data16[i]) == 'e')
					{
						if (pastExponent)
							return false;
						else
						{
							pastExponent = true;
							if ((data16[i+1] == '+') || (data16[i+1] == '-'))
								i++;
						}
					}
					else if (toLower(data16[i]) == 'i')
					{
						if (imag)
							return false;
						else
						{
							pastExponent = pastDecimal = false;
							imag = true;
						}
					}
					else if ((data16[i] == '-') || (data16[i] == '+'))
					{
						if (ir || (i >= character_ct-1))
							return false;
						else
						{
							pastDecimal = pastExponent = false;
							ir = true;
						}
					}
					else return false;
				}
			}

			return true;
		}

		///mutators
		template <>
		const string<utf16>& string<utf16>::operator=(const string<utf16>& other)
		{
			size_t new_len = (other.character_ct + 1) * this->charSize();
			this->increase(new_len);
			// data = new uint8_t[data_len];

			character_ct = other.character_ct;

			uint32_t* data32 = (uint32_t*)data;
			uint32_t* other32 = (uint32_t*)other.data;
			size_t len32 = data_len >> 2;

			for (size_t i=0; i<len32; i++)
				data32[i] = other32[i];

			size_t len = len32 << 2;
			for (size_t i=len; i<data_len; i++)
				data[i] = other.data[i];

			return *this;
		}

		template <>
		string<utf16> string<utf16>::substr(size_t index, int count) const
		{
			string<utf16> result;
			uint16_t* data16 = (uint16_t*)data;
			uint16_t* result16;

			if (count < 0)
			{
				if (index >= character_ct) index = character_ct - 1;

				count = -count;
				if ((size_t)count > index+1) count = index+1;
				result.increase((1+count) << 1);

				result16 = (uint16_t*)result.data;

				size_t beg = index - count + 1;
				for (size_t i=beg; i<=index; i++)
				{
					result16[index-i] = data16[i];
				}
				result16[count] = 0;
				result.character_ct = count;
			}
			else if (count)
			{
				if (index >= character_ct) return result;

				if ((size_t)count > index+1) count = index+1;
				result.increase((1+count) << 1);

				result16 = (uint16_t*)result.data;

				size_t end = index + count;
				for (size_t i=index; i<end; i++)
					result16[i-index] = data16[i];

				result16[count] = 0;
				result.character_ct = count;
			}

			return result;
		}

		template <>
		const string<utf16>& string<utf16>::insert(const string<utf16>& other, size_t index)//insert before index
		{
			if (!other.character_ct) return *this;

			if (index >= character_ct) index = character_ct;

			size_t start = index + other.character_ct;
			size_t end = character_ct + other.character_ct;
			this->increase(end << 1);

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			for (size_t i=end-1; i>=start; i--)
			{
				data16[i] = data16[i - other.character_ct];
			}

			end = index + other.character_ct;
			for (size_t i=index; i<end; i++)
			{
				data16[i] = other16[i-index];
			}

			character_ct += other.character_ct;
			data16[character_ct] = 0;

			return *this;
		}

		template <>
		const string<utf16>& string<utf16>::remove(size_t index, int count)
		{
			if (count)
			{
				if (index >= character_ct) return *this;

				size_t start, end, offset;

				if (count < 0)
				{
					if ((index + count) > character_ct)
					{
						start = 0;
						offset = index;
					}
					else
					{
						start = index + count;
						offset = -count;
					}
				}
				else
				{
					if ((index + count) > character_ct)
						offset = character_ct - index;
					else
						offset = count;

					start = index;
				}

				uint16_t* data16 = (uint16_t*)data;

				end = character_ct - offset;
				for (size_t i=start; i<end; i++)
				{
					data16[i] = data16[i+offset];
				}

				data16[end] = 0;
				character_ct = end;
			}

			return *this;
		}

		template <>
		const string<utf16>& string<utf16>::replace(size_t index, int count, const string<utf16>& other)
		{
			if (count)
			{
				size_t start, end;

				if (count < 0)
				{
					if ((index >= character_ct) && ((size_t)-count >= character_ct))
						return operator=(other);

					if (index >= character_ct)
					{
						start = character_ct + count;
						end = character_ct;
					}
					else
					{
						end = index + 1;

						if ((size_t)-count >= character_ct)
							start = 0;
						else
							start = end + count;
					}
				}
				else
				{
					if (index >= character_ct)
						return operator+=(other);

					if (!index && ((size_t)count >= character_ct))
						return operator=(other);

					start = index;
					if ((size_t)count >= character_ct)
						end = character_ct;
					else
						end = start + count;
				}


				size_t offset = end - start;
				size_t newCharCt = character_ct - offset + other.character_ct;
				this->increase((newCharCt + 1) << 1);

				uint16_t* data16 = (uint16_t*)data;
				uint16_t* other16 = (uint16_t*)other.data;

				if (newCharCt < character_ct)
				{
					//pull chars in
					size_t toOffs = newCharCt - character_ct;

					for (size_t i=end; i<character_ct; i++)
						data16[i+toOffs] = data16[i];
				}
				else if (newCharCt > character_ct)
				{
					//pull chars out
					size_t toPos = newCharCt + 1;
					size_t fromPos = character_ct + 1;

					for (size_t i=end; i<character_ct; i++)
						data16[toPos-i] = data16[fromPos-i];
				}
				//else just directly replace chars

				for (size_t i=0; i<other.character_ct; i++)
					data16[i+start] = other16[i];

				character_ct = newCharCt;
				data16[character_ct] = 0;
			}

			return *this;
		}

		template <>
		string<utf16> string<utf16>::upper() const
		{
			string<utf16> result (*this);
			uint16_t* data16 = (uint16_t*)result.data;

			for (size_t i=0; i<character_ct; i++)
				data16[i] = toUpper(data16[i]);

			return result;
		}

		template <>
		string<utf16> string<utf16>::lower() const
		{
			string<utf16> result (*this);
			uint16_t* data16 = (uint16_t*)result.data;

			for (size_t i=0; i<character_ct; i++)
				data16[i] = toLower(data16[i]);

			return result;
		}

		template <>
		string<utf16> string<utf16>::camel() const
		{
			string<utf16> result (*this);
			uint16_t* data16 = (uint16_t*)result.data;

			bool doLower = false;
			for (size_t i=0; i<character_ct; i++)
			{
				uint32_t ch = data16[i];
				data16[i] = (doLower ? toLower(ch) : toUpper(ch));

				doLower = isAlphaNumeric(ch) || (ch == '_');
			}

			return result;
		}

		template <>
		void string<utf16>::read(inputStream& stream, uint32_t delim)
		{
			character_ct = 0;
			this->increase(2);

			uint16_t* data16 = (uint16_t*)data;

			if (stream.bad())
			{
				data16[character_ct] = 0;
				return;
			}

			uint32_t last = stream.getChar(utf16);

			while (!stream.empty() && (delim ? (last == delim) : isWhiteSpace(last)))
				last = stream.getChar(utf16);

			while (!stream.empty() && !(delim ? (last == delim) : isWhiteSpace(last)))
			{
				data16[character_ct++] = last;
				this->increase((character_ct+1) << 1);

				last = stream.getChar(utf16);
			}

			data16[character_ct] = 0;
		}

		template <>
		void string<utf16>::readln(inputStream& stream)
		{
			character_ct = 0;
			this->increase(2);

			uint16_t* data16 = (uint16_t*)data;

			if (stream.bad())
			{
				data16[character_ct] = 0;
				return;
			}

			uint32_t last = stream.getChar(utf16);

			while (!stream.empty())
			{
				if (last == '\r')
				{
					last = stream.getChar(utf16);
					if (last == '\n')
					{
						data16[character_ct] = 0;
						return;
					}
					data16[character_ct++] = '\r';
				}
				else if (last == '\n')
				{
					data16[character_ct] = 0;
					return;
				}

				data16[character_ct++] = last;
				this->increase((character_ct+1) << 1);

				last = stream.getChar(utf16);
			}

			data16[character_ct] = 0;
		}

		template <>
		uint32_t string<utf16>::operator[](size_t index) const
		{
			return this->at(index);
		}

		template <>
		void string<utf16>::initInt(long long value, unsigned int base, unsigned int padSize)
		{
			uint8_t ibuf[Z_STR_INT_BUFSIZE];
			if ((base < 2) || (base > 36)) base = 10;

			bool negative = false;
			if (value < 0)
			{
				value = -value;
				negative = true;
			}

			size_t ibufsiz = integralBuf(value, base, ibuf);

			//initialize string data
			character_ct = ibufsiz + negative;
			if (character_ct < padSize)
				character_ct += (padSize -= character_ct);
			else
				padSize = 0;

			data_len = (character_ct + 1) * this->charSize();
			data = new uint8_t[data_len];

			if (negative) this->initChar('-', 0);

			size_t pos = negative;

			for (size_t i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (size_t i=0; i<ibufsiz; i++)
				this->initChar(ibuf[ibufsiz-i-1], pos++);
		}

		template <>
		void string<utf16>::initPointer(void* pointer)
		{
			uint8_t pbuf[Z_STR_INT_BUFSIZE];

			union ptv
			{
				void* pval;
				unsigned long ival;
			};
			ptv ptr;
			ptr.pval = pointer;

			size_t pbufsiz = integralBuf(ptr.ival, 16, pbuf);
			size_t padSize;

			//initialize string data
			if (Z_STR_POINTER_FORCE && (pbufsiz < Z_STR_POINTER_CHARS))
				padSize = Z_STR_POINTER_CHARS - pbufsiz;
			else
				padSize = 0;

			character_ct = padSize + pbufsiz + 2;
			data_len = (character_ct + 1) * this->charSize();
			data = new uint8_t[data_len];

			// if (negative) this->initChar('-', 0);

			size_t pos = 0;

			this->initChar('0',pos++);
			this->initChar('x',pos++);

			for (size_t i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (size_t i=0; i<pbufsiz; i++)
				this->initChar(pbuf[pbufsiz-i-1], pos++);
		}

		template <>
		void string<utf16>::initFloat(double value, unsigned int base, unsigned int precision, unsigned int padSize)
		{
			uint8_t ibuf[Z_STR_INT_BUFSIZE];
			uint8_t fbuf[Z_STR_FLOAT_BUFSIZE];
			uint8_t ebuf[Z_STR_EXP_BUFSIZE];

			data = 0;

			union float_cast
			{
				double fval;
				unsigned long ival;

				struct
				{
					unsigned long mantissa : 52;
					unsigned int exponent : 11;
					bool sign : 1;
				} raw;
			};
			float_cast number;
			number.fval = value;

			bool negative = number.raw.sign;
			number.raw.sign = 0;
			bool force = true;

			if ((base < 2) || (base > 36)) base = 10;
			if (precision <= 0)
			{
				precision = Z_STR_FLOAT_PRECISION;
				force = false;
			}

			//integral and fractional parts of the number
			unsigned long integral;
			double fractional;
			unsigned long exponent = 0;
			bool negexponent = false;

			if (number.ival) //don't bother with exponents if 0
			{
				double temp = number.fval;
				unsigned long tempExp = exponent;
				bool tempNegExp = negexponent;

				if (1023 <= number.raw.exponent)// pos exponent
				{
					while (temp >= base)
					{
						temp /= base;
						tempExp++;
					}
				}
				else if (1023 >= number.raw.exponent)// neg exponent
				{
					tempNegExp = true;
					double frac = 1.0 / (double)base;

					while (temp < frac)
					{
						temp *= base;
						tempExp++;
					}
				}

				if ((tempNegExp && (tempExp >= precision)) ||
					(number.fval > (double)(INT_MAX)))
				{
					number.fval = temp;
					exponent = tempExp;
					negexponent = tempNegExp;
				}
			}

			if (number.raw.exponent < 1023)//x2^neg
			{
				integral = 0;
			}
			else if (number.raw.exponent > 1023)//x2^(pos)
			{
				long expo = number.raw.exponent - 1023;
				integral = ((long)1 << expo) + (number.raw.mantissa >> ((long)52 - expo));
			}
			else //x2^0
			{
				integral = 1;
			}

			fractional = number.fval - (double)integral;

			//this section rounds to nearest fractional point (decimal pt. in base 10)
			if (number.ival)
			{
				//prep for rounding
				double tmp = fractional;
				double roundval = tmp / (double)(base << 1);
				double roundadd = roundval;
				for (size_t i=0; i<precision; i++)
				{
					tmp *= base;
					tmp -= (double)(long)tmp;
					roundadd /= base;
				}
				//round
				if (tmp >= roundval)
				{
					fractional += roundadd;

					if (fractional >= 1)
					{
						integral++;
						fractional--;
					}
				}
			}

			// number.raw.exponent - 1023;
			size_t ibufsiz = integralBuf(integral, base, ibuf);
			size_t fbufsiz = fractionalBuf(fractional, base, precision, force, fbuf);
			// size_t fbufsiz = fractionalBuf(fractional, 10, 6, 0, fbuf);
			size_t ebufsiz = exponent ? integralBuf(exponent, base, ebuf) : 0;
			//initialize string data
			character_ct = ibufsiz + negative + (bool)fractional + fbufsiz + (bool)exponent + negexponent + ebufsiz;
			if (character_ct < padSize)
				character_ct += (padSize -= character_ct);
			else
				padSize = 0;

			// bool

			// character_ct = fbufsiz;

			// character_ct = fbufsiz + (bool)fbufsiz;
			data_len = (character_ct + 1) * this->charSize();
			// data_len = 100;
			// character_ct = 0;
			data = new uint8_t[data_len];
			// data = new uint8_t[10];
			// data_len = 10;
			// character_ct = 0;
			if (negative) this->initChar('-', 0);
			// size_t pos = 0;
			size_t pos = negative;
			//
			for (size_t i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (size_t i=0; i<ibufsiz; i++)
				this->initChar(ibuf[ibufsiz-i-1], pos++);

			if (fbufsiz)
			{
				this->initChar('.', pos++);

				for (size_t i=0; i<fbufsiz; i++)
					this->initChar(fbuf[i],pos++);
			}

			if (exponent)
			{
				this->initChar('e', pos++);
				if (negexponent)
					this->initChar('-', pos++);

				for (size_t i=0; i<ebufsiz; i++)
					this->initChar(ebuf[ebufsiz-i-1],pos++);
			}

			this->initChar(0,pos);
		}

		template <>
		void string<utf16>::initComplex(const std::complex<double>& value, unsigned int base, unsigned int precision)
		{
			data_len = 4;
			data = new uint8_t[data_len];
			*((uint32_t*)data) = 0;
			character_ct = 0;

			if (value.real() && value.imag())
			{
				operator=(string<utf32>(value.real(), base, precision, 0));
				if (value.imag() > 0) operator+=("+");
				operator+=(string<utf32>(value.imag(), base, precision, 0));
				operator+=("i");
			}
			else if (value.imag())
			{
				operator=(string<utf32>(value.imag(), base, precision, 0));
				operator+=("i");
			}
			else
			{
				operator=(string<utf32>(value.real(), base, precision, 0));
			}

		}

		template <>
		size_t string<utf16>::size() const
		{
			return sizeof(*this) + (data_len * sizeof(uint8_t));
		}

		template <>
		size_t string<utf16>::length() const
		{
			return character_ct;
		}

		template <>
		size_t string<utf16>::chars() const
		{
			return character_ct;
		}

		template <>
		long string<utf16>::integer(int base) const
		{
			if ((base < 2) || (base > 36)) return 0;

			bool negative = (data[0] == '-');
			long result = 0;

			size_t start = (negative || (data[0] == '+'));

			for (size_t i=start; i<character_ct; i++)
			{
				uint32_t chr = data[i];

				if (isNumeric(chr))
				{
					result *= base;
					result += numeralValue(chr);
				}
				else return 0;
			}

			return (negative ? -result : result);
		}

		template <>
		double string<utf16>::floating(int base) const
		{
			if ((base < 2) || (base > 36)) return 0;

			if (!character_ct) return 0;

			bool pastDecimal, pastExponent, negexponent;
			pastDecimal = pastExponent = negexponent = false;

			bool negative = (data[0] == '-');
			size_t start = (negative || (data[0] == '+'));

			if (start >= character_ct) return 0;

			double result = 0;
			double frac = 1;
			int exponent = 0;

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], base))
				{
					if (data[i] == '.')
					{
						if (pastDecimal || pastExponent)
							return 0;
						else
						{
							if (i >= character_ct-1) return false;
							pastDecimal = true;
						}
					}
					else if (toLower(data[i]) == 'e')
					{
						if (pastExponent)
							return 0;
						else
						{
							pastExponent = true;
							negexponent = (data[i+1] == '-');
							if (negexponent || (data[i+1] == '+'))
								i++;
						}
					}
					else return 0;
				}
				else
				{
					if (pastExponent)
					{
						exponent *= base;
						exponent += numeralValue(data[i]);
					}
					else if (pastDecimal)
					{
						frac /= base;
						result += (double)numeralValue(data[i])*frac;
					}
					else
					{
						result *= base;
						result += numeralValue(data[i]);
					}
				}
			}

			if (pastExponent)
			{
				for (int i=0; i<exponent; i++)
				{
					if (negexponent)
						result /= base;
					else
						result *= base;
				}
			}

			return (negative ? -result : result);
		}

		template <>
		std::complex<double> string<utf16>::complex(int base) const
		{
			if ((base < 2) || (base > 36)) return 0;

			if (!character_ct) return 0;

			bool pastDecimal, pastExponent, imag, ir, negexponent;
			pastDecimal = pastExponent = imag = ir = negexponent = false;

			bool negative = (data[0] == '-');
			size_t start = (negative || (data[0] == '+'));

			if (start >= character_ct) return 0;

			double imagResult = 0;
			double realResult = 0;
			double result = 0;
			double frac = 1;
			int exponent = 0;

			if (negative) result = -result;

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], base))
				{
					if (data[i] == '.')
					{
						if (pastDecimal || pastExponent)
							return 0;
						else
						{
							if ((i >= character_ct-1) || (toLower(data[i+1]) == 'i'))
								return 0;
							pastDecimal = true;
						}
					}
					else if (toLower(data[i]) == 'e')
					{
						if (pastExponent)
							return 0;
						else
						{
							pastExponent = true;

							negexponent = (data[i+1] == '-');
							if (negexponent || (data[i+1] == '+'))
								i++;
						}
					}
					else if (toLower(data[i]) == 'i')
					{
						if (imag)
							return 0;
						else
						{
							imag = true;

							if (!i || !isNumeric(data[i-1], base)) result = 1;

							if (pastExponent)
							{
								for (int i=0; i<exponent; i++)
								{
									if (negexponent)
										result /= base;
									else
										result *= base;
								}
							}

							pastExponent = pastDecimal = negexponent = false;

							imagResult = (negative ? -result : result);
							result = 0;
						}
					}
					else if ((data[i] == '-') || (data[i] == '+'))
					{
						if (ir || (i >= character_ct-1))
							return 0;
						else
						{
							ir = true;

							if (!imag)
							{
								if (pastExponent)
								{
									for (int i=0; i<exponent; i++)
									{
										if (negexponent)
											result /= base;
										else
											result *= base;
									}
								}

								realResult = (negative ? -result : result);
								result = 0;
							}

							pastDecimal = pastExponent = negexponent = false;

							negative = (data[i] == '-');
						}
					}
					else return 0;
				}
				else
				{
					if (pastExponent)
					{
						exponent *= base;
						exponent += numeralValue(data[i]);
					}
					else if (pastDecimal)
					{
						frac /= base;
						result += (double)numeralValue(data[i])*frac;
					}
					else
					{
						result *= base;
						result += numeralValue(data[i]);
					}
				}
			}

			if (pastExponent)
			{
				for (int i=0; i<exponent; i++)
				{
					if (negexponent)
						result /= base;
					else
						result *= base;
				}
			}

			if (result)
				realResult = (negative ? -result : result);

			return std::complex<double>(realResult, imagResult);
		}

		///analyzers
		template <>
		bool string<utf16>::foundAt(const string<utf16>& other, size_t index) const
		{
			if ((character_ct - index) < other.character_ct) return false;

			uint32_t* data32 = (uint32_t*)data;
			uint32_t* other32 = (uint32_t*)other.data;

			const size_t charSz = this->charSize();

			size_t i = 0;
			size_t idx = index * charSz;
			size_t end = (other.character_ct * charSz) >> 2;
			for (i=0; i<end; i++)
			{
				if (data32[i+index] != other32[i])
					return false;
			}

			for (i=(end<<2); i<(other.character_ct * charSz); i++)
			{
				if (data[i+idx] != other[i])
					return false;
			}

			return true;
		}

		template <>
		bool string<utf16>::beginsWith(const string<utf16>& other) const
		{
			return this->foundAt(other, 0);
		}

		template <>
		bool string<utf16>::foundEndAt(const string<utf16>& other, size_t index) const
		{
			if (index < other.character_ct) return false;
			if (index >= character_ct) return false;

			const size_t charSz = this->charSize();
			const size_t idx = (index-other.character_ct+1)*charSz;
			const size_t last = other.character_ct * charSz;

			uint32_t* data32 = (uint32_t*)&data[idx];
			uint32_t* other32 = (uint32_t*)other.data;

			size_t i = 0;
			size_t end = last >> 2;
			while (i < end)
			{
				if (data32[i] != other32[i])
					return false;

				i++;
			}

			for (i=(end<<2); i<last; i++)
			{
				if (data[i+idx] != other[i])
					return false;
			}

			return true;
		}

		///mutators
		template <>
		const string<utf16>& string<utf16>::remove(const string& other, int occurrence)
		{
			if (occurrence > 0) //remove one occurrence
			{
				int pos = this->find(other, occurrence);

				if (pos >= 0)
				{
					this->remove(pos, other.length());
				}
			}
			else if (!occurrence) //remove all occurrences
			{
				int pos = this->find(other, 1);

				while (pos >= 0)
				{
					this->remove(pos, other.length());
					pos = this->findAfter(other, pos, 1);
				}
			}

			return *this;
		}

		template <>
		const string<utf16>& string<utf16>::replace(const string<utf16>& findStr, const string<utf16>& replStr, int occurrence)
		{
			if (occurrence > 0) //replace one occurrence
			{
				int pos = this->find(findStr, occurrence);

				if (pos >= 0)
				{
					this->replace((size_t)pos, (int)findStr.length(), replStr);
				}
			}
			else if (!occurrence) //replace all occurrences
			{
				int pos = this->find(findStr, 1);

				while (pos >= 0)
				{
					this->replace((size_t)pos, (int)findStr.length(), replStr);
					pos = this->findAfter(findStr, pos+replStr.length(), 1);
				}
			}

			return *this;
		}

		template <>
		const string<utf16>& string<utf16>::padLeft(const string<utf16>& other, size_t padSize)
		{
			if (padSize <= character_ct) return *this;

			string<utf16> padStr;

			size_t padChars = padSize - character_ct;

			while (padChars >= other.character_ct)
			{
				padStr += other;
				padChars -= other.character_ct;
			}

			if (padChars)
				padStr += other.substr(0, padChars);

			return this->insert(padStr, 0);
		}

		template <>
		const string<utf16>& string<utf16>::padRight(const string<utf16>& other, size_t padSize)
		{
			if (padSize <= character_ct) return *this;

			string<utf16> padStr;

			size_t padChars = padSize - character_ct;

			while (padChars >= other.character_ct)
			{
				padStr += other;
				padChars -= other.character_ct;
			}

			if (padChars)
				padStr += other.substr(0, padChars);

			return operator+=(padStr);
		}

		template <>
		const string<utf16>& string<utf16>::trimLeft(const string<utf16>& other)
		{
			if (character_ct < other.character_ct) return *this;

			size_t index = 0;
			while (this->foundAt(other, index))
			{
				index += other.character_ct;
			}

			return this->remove(0, index);
		}

		template <>
		const string<utf16>& string<utf16>::trimRight(const string<utf16>& other)
		{
			if (character_ct < other.character_ct) return *this;

			size_t index = character_ct-other.character_ct;
			int count = 0;
			while (this->foundAt(other, index))
			{
				index -= other.character_ct;
				count += other.character_ct;
			}

			return this->remove(index+other.character_ct, character_ct);
		}

		template <>
		const string<utf16>& string<utf16>::cutDuplicates(const string<utf16>& other)
		{
			int pos = this->find(other, 1);

			while (pos >= 0)
			{
				size_t opos = pos + other.length();
				while (this->foundAt(other, opos))
				{
					this->remove(opos, other.length());
				}
				pos = this->findAfter(other, opos, 1);
			}

			return *this;
		}

		///operators
		template <>
		string<utf16> string<utf16>::operator+(const string<utf16>& other) const
		{
			string<utf16> result (*this);
			result += other;

			return result;
		}

		template <>
		bool string<utf16>::operator==(const string<utf16>& other) const
		{
			if (character_ct != other.character_ct)
				return false;

			size_t fast_sz = (character_ct << 1) / sizeof(size_t);
			size_t slow_bg = (fast_sz * sizeof(size_t)) >> 1;

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] != other_ptr[i])
					return false;
			}

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			//check any remaining characters
			for (size_t i=slow_bg; i<character_ct; i++)
			{
				if (data16[i] != other16[i])
					return false;
			}

			return (character_ct < other.character_ct);
		}

		template <>
		bool string<utf16>::operator>(const string<utf16>& other) const
		{
			size_t max_char;
			if (character_ct < other.character_ct)
				max_char = character_ct;
			else
				max_char = other.character_ct;

			size_t fast_sz = (max_char << 1) / sizeof(size_t);
			size_t slow_bg = (fast_sz * sizeof(size_t)) >> 1;

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] > other_ptr[i])
					return true;
				else if (data_ptr[i] < other_ptr[i])
					return false;
			}

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			//check any remaining characters
			for (size_t i=slow_bg; i<max_char; i++)
			{
				if (data16[i] > other16[i])
					return true;
				else if (data16[i] < other16[i])
					return false;
			}

			return (character_ct > other.character_ct);
		}

		template <>
		bool string<utf16>::operator<(const string<utf16>& other) const
		{
			size_t max_char;
			if (character_ct < other.character_ct)
				max_char = character_ct;
			else
				max_char = other.character_ct;

			size_t fast_sz = (max_char << 1) / sizeof(size_t);
			size_t slow_bg = (fast_sz * sizeof(size_t)) >> 1;

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] < other_ptr[i])
					return true;
				else if (data_ptr[i] > other_ptr[i])
					return false;
			}

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			//check any remaining characters
			for (size_t i=slow_bg; i<max_char; i++)
			{
				if (data16[i] < other16[i])
					return true;
				else if (data16[i] < other16[i])
					return false;
			}

			return (character_ct < other.character_ct);
		}

		template <>
		void string<utf16>::write(outputStream& stream) const
		{
			if (stream.bad()) return;

			if (character_ct)
				stream.put(data, character_ct, utf16);
		}

		template <>
		void string<utf16>::writeln(outputStream& stream) const
		{
			if (stream.bad()) return;

			if (character_ct)
				stream.put(data, character_ct, utf16);

			string<utf16> newl = "\n";
			stream.put(newl.data, newl.character_ct, utf16);
		}

		template <>
		void string<utf16>::serialIn(inputStream& stream)
		{
			if (stream.bad() || !stream.binary())
			{
				character_ct = 0;
				this->increase(4);
				*((uint32_t*)data) = 0;
				return;
			}

			size_t datact = 0;
			core::serialIn(datact, stream);
			character_ct = datact / this->charSize();
			this->increase(datact + 4);

			size_t i = 0;
			while (!stream.empty() && (i < datact))
			{
				data[i++] = stream.get();
			}

			*((uint32_t*)&data[i]) = 0;
		}

		template <>
		void string<utf16>::serialOut(outputStream& stream) const
		{
			if (stream.bad() || !stream.binary())
				return;

			size_t datact = character_ct * this->charSize();
			core::serialOut(datact, stream);

			size_t i = 0;
			while ((i < datact))
			{
				stream.put(data[i++]);
			}
		}

	} //end of core namespace
} //end of z namespace
