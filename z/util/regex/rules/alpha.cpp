#include "alpha.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			alpha::alpha(bool negate, int min, int max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool alpha::match(uint32_t current) const
			{
				return negate ^ core::isAlpha(current);
			}

#			ifdef DEBUG
			void alpha::print(core::outputStream& stream, int level)
			{
				(zpath().padLeftIn(" ",(level)<<1)+(negate?"\\A":"\\a")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
