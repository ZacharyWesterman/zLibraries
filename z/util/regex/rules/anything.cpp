#include "anything.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			anything::anything(bool newline, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), newline(newline) {}

			bool anything::match(core::inputStream& stream) const noexcept
			{
				if (stream.empty()) return false;

				auto pos = stream.tell();
				auto ch = stream.getChar();
				if (stream.empty()) return false;
				if (newline)
				{
					if (ch == '\n')
					{
						pos = stream.tell();
						ch = stream.getChar();
						if (stream.empty()) return true;
						if (ch != '\r') stream.seek(pos);
						return true;
					}
					else if (ch == '\r')
					{
						pos = stream.tell();
						ch = stream.getChar();
						if (stream.empty()) return true;
						if (ch != '\n') stream.seek(pos);
						return true;
					}
				}
				else
				{
					if ((ch != '\n') && (ch != '\r')) return true;
				}

				stream.seek(pos);
				return false;
			}

			bool anything::base() const noexcept
			{
				return false;
			}

#			ifdef DEBUG
			void anything::print(core::outputStream& stream, int level) noexcept
			{
				(zstring(" ").repeat(level*2)+"."+meta()).writeln(stream);
			}
#			endif
		}
	}
}
