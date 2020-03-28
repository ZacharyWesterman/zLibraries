#pragma once
#include <z/core/stream.hpp>

#ifdef DEBUG
#include <z/core/string.hpp>
#endif

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \interface rule
			 * \brief Regex rule interface.
			 *
			 * This class contains default data that any regex rule must contain,
			 * and sets default rule behavior.
			 */
			class rule
			{
			public:
				/**
				 * \brief Default full constructor.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				rule(size_t min=1, size_t max=1, bool greedy=true): min(min), max(max), greedy(greedy){}

				///Default destructor.
				virtual ~rule(){}

				/**
				 * \brief Attempt to match a single character with this rule.
				 * \param current The character to match against.
				 * \return True if the character fits the criteria of this rule, false otherwise.
				 */
				virtual bool match(uint32_t current) const
				{
					(void)current;
					return false;
				}

				/**
				 * \brief Attempt to match this rule to a set of characters on the stream, and/or the stream's state.
				 * \param stream The stream to match against.
				 * \return True if the rule matched, consuming all matched input. Otherwise returns false and does not consume input.
				 */
				virtual bool match(core::inputStream& stream) const
				{
					(void)stream;
					return false;
				}

				/**
				 * \brief Check whether this rule can accurately determine a result with a single character as input.
				 * \return True if this rule needs only one character, false otherwise.
				 */
				virtual bool base() const {return true;}

				/**
				 * \brief Check if this is a parent rule.
				 * \return True if this rule can have child rules (e.g. rgx::andlist, rgx::orlist, etc.), false otherwise.
				 */
				virtual bool parent() const {return false;}

				/**
				 * \brief Check if this rule is an OR group.
				 * \return True only if the rule is of rgx::orlist type, false otherwise.
				 */
				virtual bool isOr() const {return false;}

				///The minimum number of times this rule should match.
				size_t min;

				///The maximum number of times this rule can match.
				size_t max;

				/**
				 * \brief Whether this rule should consume input greedily.
				 *
				 * If false, will match until a sibling rule meets its minimum.
				 * If true, this rule matches as many times as it can.
				 */
				bool greedy;

#			ifdef DEBUG
				/**
				 * \brief Debug function to write all metadata about a rule to a stream.
				 * \param stream The stream to write to.
				 * \param level How many levels the text should be indented.
				 */
				virtual void print(core::outputStream& stream, int level=0)=0;

				/**
				 * \brief Debug function to print extra metadata about a regex rule.
				 * \return A string showing the rule's count range and whether it is greedy.
				 */
				zpath meta()
				{
					zpath s = ' ';
					if ((min != 1) || (max != 1))
					{
						s += '{';
						s += min;
						s += ',';
						s += (int)max;
						s += '}';
					}
					if (!greedy)
					{
						s += ' ';
						s += '?';
					}
					return s;
				}
#			endif
			};

		}
	}
}