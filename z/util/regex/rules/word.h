#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class word : public rule
			{
			public:
				bool match(uint32_t current) const;
			};
		}
	}
}