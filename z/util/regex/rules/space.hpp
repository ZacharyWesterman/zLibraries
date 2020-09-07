#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching white space.
			 */
			class space : public rule
			{
			public:
				/**
				 * \brief Full constructor with negate option.
				 * \param negate Whether to perform an inverse match.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				space(bool negate=false, int min=1, int max=1, bool greedy=true) noexcept;
				bool match(uint32_t current) const noexcept;

				///If true, only matches non-whitespace characters.
				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
