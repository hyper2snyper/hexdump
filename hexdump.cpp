#include <iostream>
#include <iomanip>
#include <fstream>


unsigned int digit_len(unsigned int val)
{
	unsigned int digits{1};
	while(val >= 0x10)
	{
		digits++;
		val /= 0x10;
	}
	return digits;

}

void print_with_padding(unsigned int val, unsigned int len)
{
	unsigned int pad{len-digit_len(val)};
	for(size_t i{0}; i<pad; ++i)
	{
		std::cout << 0;
	}
	std::cout << std::hex << val;
}

bool str_eq(unsigned char* a, unsigned char* b, unsigned int len)
{
	for(size_t i{0}; i < len; ++i)
	{
		if(a[i] != b[i]) return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	if(argc <= 1)
	{
		std::cerr << "No file path.\n";
		return 1;
	}

	std::ifstream file{argv[1], std::ios::ate | std::ios::binary};
	file.unsetf(std::ios_base::skipws);

	if(!file.is_open())
	{
		std::cerr << "Invalid file path.\n";
		return 1;
	}

	unsigned int total{file.tellg()};
	file.seekg(0, file.beg);
	unsigned int pad{digit_len(total)};


	unsigned char bits[2][0x10]{};
	unsigned char* prev = nullptr;
	unsigned char* curr = bits[0];
	bool index{};
	bool equal{false};

	for(size_t i{0}; i<(total/0x10)+1; ++i)
	{

		for(size_t j{0}; j<0x10; ++j)
		{
			unsigned char val{};
			if(!file.eof())
			{
				val = file.get();
			}
			curr[j] = val;
		}

		if(prev != nullptr && str_eq(curr, prev, 0x10))
		{
			if(!equal)
			{
				std::cout << "*\n";
			}
			equal = true;
		} else {
			equal = false;
			print_with_padding(i, pad-1);
			std::cout << "0 : ";
			for(size_t j{0}; j<0x10; ++j)
			{
				if((i*0x10)+j >= total)
				{
					std::cout << "~~ ";
					continue;
				}
				print_with_padding((unsigned int) curr[j], 2);
				std::cout << " ";
			}

			std::cout << "  |";
			for(size_t j{0}; j<0x10; ++j)
			{
				char to_print{curr[j]};
				if(to_print < 0x20)
				{
					to_print = ' ';
				}
				std::cout << to_print << " ";
				
			}
			std::cout << "|\n";
		}
		prev = curr;
		index = !index;
		curr = bits[(int) index];
	}

	std::cout << total-1;

	
	

	return 0;

}
