/*
I: if_sub if_role if_map
O: ofs
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>


std::map<std::string, std::string> preMap(std::ifstream &mf) {
	std::map<std::string, std::string> r_map;
	std::string key, name;
	while (mf >> key && getline(mf, name)) {
		try {
			if (name.size() > 1) {
				r_map[key] = name.substr(1);
			} else 
				throw std::runtime_error("Key " + key + " doesn't have any replaced value.");
		} catch(std::runtime_error err) {
			std::cerr << err.what() << std::endl;
		}
	}
	return r_map;
}

int main(void) {
	// @move on using commandline to set i/o file instead changing at here
	std::ifstream if_sub("/Users/sytu/workspace/c++/srtParser/use_regex/in/sv_s1e1_sub.srt");
	if (!if_sub) {
		std::cerr << "error with input sub file" << std::endl; 
		return -1;
	}
	std::ifstream if_role("/Users/sytu/workspace/c++/srtParser/use_regex/in/sv_s1e1_role.txt");
	if (!if_role) {
		std::cerr << "error with input role file" << std::endl; 
		return -1;
	}
	std::ifstream if_map("/Users/sytu/workspace/c++/srtParser/use_regex/in/sv_s1e1_map.txt");
	if (!if_map) {
		std::cerr << "error with input map file" << std::endl; 
		return -1;
	}
	// if_3
	std::ofstream of("/Users/sytu/workspace/c++/srtParser/use_regex/out/sv_s1e1_o.txt");
	if (!of) {
		std::cerr << "error with output file" << std::endl; 
		return -1;
	}
	
	// class             
	// SrtParser sp(if_sub, if_role, if_map, of);

	// read data
	std::stringstream ss;
	ss << if_sub.rdbuf();
	std::string data(ss.str());

	// get map
	auto trans_map = preMap(if_map);

	// parse data
	std::regex regex(R"((\d+)\r\n(\d+:\d{2}:\d{2},\d{3})\s-->\s(\d+:\d{2}:\d{2},\d{3})\r\n(.*)\r\n\r\n)");
	std::match_results<std::string::const_iterator> match_results;
	auto begin = data.cbegin();
	std::vector<std::tuple<std::string, std::string, std::string, std::string>> vec;
	while (begin != data.cend()) {
		if (std::regex_search(begin, data.cend(), match_results, regex)) {
			vec.emplace_back(match_results[1], match_results[2], match_results[3], match_results[4]);
			begin = match_results.suffix().first;
		}
		else {
			break;
		}
	}

	for (const auto& t : vec) {
		std::string role;
		getline(if_role, role);
		of << trans_map.find(role)->second << ": \t\t" << std::get<3>(t) << std::endl << std::endl;
	}

}

// for (const auto& t : vec)
	// {
	//   std::cout << "序号: " << std::get<0>(t)
	//     << " 开始时间: " << std::get<1>(t)
	//     << " 结束时间: " << std::get<2>(t)
	//     << " 文本: " << std::get<3>(t)
	//     << std::endl;
	// }