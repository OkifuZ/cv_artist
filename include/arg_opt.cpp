#include <vector>
#include <string>

int get_all_opt(int argc, char* argv[], std::vector<std::vector<std::string>>& opts) {
	int i = 1;
	opts.clear();
	std::vector<std::string> group;
	while (i < argc) {
		std::string cur_arg(argv[i]);
		if (!group.empty() && cur_arg[0] == '-') {
			opts.push_back(group);
			group.clear();
		}
		group.push_back(cur_arg);
		i++;
	}
	opts.push_back(group);
	return 0;
}
