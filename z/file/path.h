#pragma once

#include <z/core/string.h>

namespace z
{
	namespace file
	{
		/**
		 * \brief Get the full file path from a relative path.
		 *
		 * \param filePath The path to a file object.
		 * \return A string containing the full path to that file object.
		 * \threadsafe_function_yes
		 */
		zpath path(const zpath& filePath);
	}
}
