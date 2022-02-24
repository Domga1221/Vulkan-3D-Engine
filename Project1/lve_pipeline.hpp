#pragma once

#include <string>
#include <vector>

namespace lve {
	class LvePipeline {
	public:
		LvePipeline(const std::string& vertFilepath, const std::string& fragFilepath);

	private:
		static std::vector<char> readFile(const std::string& filePath); // returns vector of characters

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath); // helper function
	};
}
