#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>

bool isVariableOnBuild = false;
template<typename T, size_t N> T* end(T (&ra)[N]) { return ra + N; }
const char* _Symbols[] = {">", "<", "-", "+", ".", "&", "#", " ", "[", "]"};

int len(std::string str) {
   int length = 0;
   for(int i=0; str[i]!=0; i++) {
      length++;
   }
   return length;
}

std::string tostr(int num) {
	std::stringstream ss;
	std::string val;
	ss << num;
	ss >> val;
	return val;
}

int toint(std::string str) {
   std::stringstream ss;
   int value;
   ss << str;
   ss >> value;
   return value;
}

class Error {
	public:
		static void IllegalCharacterError(std::string linestr, int pos, int linepos) { 
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "IllegalCharError at line " << linepos << std::endl;;
		}
		static void TypeError(std::string linestr, int pos, int linepos) { 
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "TypeError at line " << linepos << std::endl;;
		}
		static void TypeMissingError(std::string linestr, int pos, int linepos) { 
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "TypeMissingError at line " << linepos << std::endl;;
		}
		static void OnUpcomingFeautures(std::string linestr, int pos, int linepos) { 
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "UpcomingFeautures at line " << linepos << std::endl;;
		}
		static void MultiplyError(std::string linestr, int pos, int linepos) { 
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "MultiplyError at line " << linepos << std::endl;
			std::cout << "You're currently on multiply-loop" << std::endl;
		}
		static void NoSuchVariable(std::string linestr, int pos, int linepos, std::string varname) {
			std::cout << linestr << std::endl;
			for(int i=0; i<pos; i++) std::cout << " ";
			std::cout << "^" << std::endl;
			std::cout << "No Variable Found With Name '" << varname << "' at line " << linepos << std::endl;
			std::cout << "Please define the variable first" << std::endl;
		}
};

// Compiler Section
int current_value = 0, error_pos = 0, error_Ln = 0;
std::vector<int> decval, mul_registry; std::vector<std::string> lines;
std::vector<std::vector<int> > mul_point;
int mul_start = 0, mul_total = 0, mul_value = 0;
bool isMulMode = false;

std::string CharBuilder(std::vector<int> decimal_code) {
	std::string Characterized;
	if(std::count(decimal_code.begin(), decimal_code.end(), -1)) {
		Error::IllegalCharacterError(lines[error_Ln - 1], error_pos, error_Ln);
		return "";
	} else {
		for(int i=0; i<decimal_code.size(); i++) {
			Characterized.push_back((char) decimal_code[i]);
		}
	}
	return isVariableOnBuild ? "" : Characterized;
}

void print(std::string str) { std::cout << str << std::endl; }
void print(std::vector<std::string> arr) { 
	std::cout << "["; for(int i=0; i<arr.size(); i++) {
		std::cout << arr[i] << ((i != arr.size() - 1) ? ", " : "");
	} std::cout << "]" << std::endl;
}
void print(std::vector<int> arr, bool enter_after = true) { 
	std::cout << "["; for(int i=0; i<arr.size(); i++) {
		std::cout << arr[i] << ((i != arr.size() - 1) ? ", " : "");
	} std::cout << "]" << (enter_after ? "\n" : "");
}
void print(std::vector<std::vector<int> > arr) { 
	std::cout << "["; for(int i=0; i<arr.size(); i++) {
		print(arr[i], false); std::cout << (i != arr.size() - 1 ? ", " : ""); 
	} std::cout << "]" << std::endl;
}

std::string replaceAll(std::string str, const std::string &from, const std::string &to) {
	size_t pos = 0;
	while((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
	return str;
}

std::string removeSpace(std::string _Str) {
	std::string _Tmp;
	for(int i=0; i<len(_Str); i++) if(_Str[i] != ' ') _Tmp += _Str[i];
	return _Tmp;
}

std::vector<std::string> split(std::string str, std::string separator) {
   std::vector<std::string> arr;
   size_t pos = 0;
   std::string token;

   while((pos = str.find(separator)) != std::string::npos) {
      token = str.substr(0, pos);
      arr.push_back(token);
      str.erase(0, pos + separator.length());
   }

   arr.push_back(str);
   return arr;
} 

int getIndex(std::vector<std::string> _Vect, std::string _Key) {
	int idx = 0; 
	for(int i=0; i<_Vect.size(); i++) if(_Vect[i] != _Key) {
		idx++;
	} else return idx;
	return -1;
}

int __IDFlag = 0;
std::vector<std::string> _Varname, _Value;
std::vector<int> _Id;
class Variable {
	public:
		static void Register(std::string _Tk) {
			_Varname.push_back(_Tk);
			_Id.push_back(++__IDFlag); 
		}

		static void RegisterValue(std::string _Val) { _Value.push_back(replaceAll(_Val, ".", "")); }
		static void Status() { 
			std::cout << "Varname: "; print(_Varname);
			std::cout << "Id: "; print(_Id);
			std::cout << "Value: "; print(_Value);
		}
		static std::vector<std::string> ValueOf(std::string _Ln, int _Pos, int _LinePos) {
			std::vector<std::string> _Collection, _Symbol(_Symbols, end(_Symbols));
			std::string _NInit = "";

			for(int i=_Ln.find("&") + 1; i<len(_Ln); i++) {
				std::string _TmpStr(1, _Ln[i]);
				if(!(std::find(_Symbol.begin(), _Symbol.end(), _TmpStr) != _Symbol.end())) {
					_NInit += _TmpStr;
				} else break;
			}
			_Collection.push_back(_NInit);
			
			if(_Varname.empty()) Error::NoSuchVariable(_Ln, 1, _LinePos, _NInit);
			else {
				if(getIndex(_Varname, _NInit) == -1) Error::NoSuchVariable(_Ln, _Pos+1, _LinePos, _NInit);
				else _Collection.push_back(_Value[getIndex(_Varname, _NInit)]);
			}
			return _Collection;
		}
};

class Parser {
	private: std::string value;

	public:
	Parser(std::string value) { this->value = value; }

	std::string _VarAssign(std::string _Line, int _LinePos) {
		using namespace std;
		isVariableOnBuild = true;
		string _Token = "", _Ln = replaceAll(removeSpace(_Line), ";", ".");

		for(int pos=1; pos<len(_Ln); pos++) {
			if(_Ln[pos] != '=') _Token += _Ln[pos];
			else if(_Ln[pos] == '=') {
				Variable::Register(_Token);
				_Token = "";
			}
			else Error::IllegalCharacterError(_Ln, pos, _LinePos);
		}

		Variable::RegisterValue(_Token);
		isVariableOnBuild = false;
		return "";
	}

	std::vector<int> Parse(int current_line = 1) {
		if(this->value.find("!BASE") != std::string::npos) {
			if(split(this->value, "-").size() > 1) {
				if(split(this->value, "-")[1] == "NUMBER") {
					Error::OnUpcomingFeautures(this->value, 6, current_line);
				}
				else if(split(this->value, "-")[1].find("$") != std::string::npos) current_value += toint(split(this->value, "$")[1]);
				else Error::TypeError(this->value, 6, current_line);
			}
			else Error::TypeMissingError(this->value, 5, current_line);
		}
		else for(int x=0; x<this->value.length(); x++) {
			if(this->value[x] == '+') { if(isMulMode) mul_registry.push_back(16); else current_value += 16; }
			else if(this->value[x] == '-') { if(isMulMode) mul_registry.push_back(-8); else current_value += -8; }
			else if(this->value[x] == '>') { if(isMulMode) mul_registry.push_back(2); else current_value += 2; }
			else if(this->value[x] == '<') { if(isMulMode) mul_registry.push_back(-1); else current_value += -1; }
			else if(this->value[x] == '.') {
				if(!isVariableOnBuild) decval.push_back(current_value);
				current_value = 0;
				current_line++;
			} else if(this->value[x] == ',') {
				if(!isVariableOnBuild) decval.push_back(current_value);
				current_line++;
			}else if(this->value[x] == ' ') {
				continue;
			} else if(this->value[x] == '[') {
				isMulMode = true;
			} else if(this->value[x] == ']') {
				mul_start = mul_registry[0];
				for(int i=1; i<mul_registry.size(); i++) {
					mul_total += mul_registry[i];
				}
				mul_value = mul_start * mul_total;
				current_value += mul_value;
				mul_value = 0;
				mul_start = 0;
				mul_total = 0;
				mul_registry.clear();
				isMulMode = false;
			} else if(this->value[x] == '#') {
				break;
			} else if(this->value[x] == '=') {
				std::cout << "line " << current_line << ": " << current_value << std::endl;
			} else if(this->value[x] == '$') {
				// check if there is an variable call
				if(this->value.find("&") != std::string::npos) {
					std::string _VarnameCall = "";
					int i;
					for(i=this->value.find("&")+1; i<this->value[i]!=0; i++) {
						if(this->value[i] == ',' || this->value[i] == '.' || this->value[i] == ';') break;
						else {
							_VarnameCall.push_back(this->value[i]);
						}
					}

					// check if there is an variable name in stored variale
					if(getIndex(_Varname, _VarnameCall) == -1) Error::NoSuchVariable(this->value, this->value.find("&")+1, current_line, _VarnameCall);
					else {
						std::vector<std::string> __ENTRY__ = Variable::ValueOf(_VarnameCall, this->value.find("&")+1, current_line);
						std::string _Tmp = replaceAll(this->value, "&"+__ENTRY__[0], __ENTRY__[1]);
						this->value = _Tmp;
						isMulMode = false;
						x = -1;
					}
				}
				this->value = _VarAssign(this->value, current_line);
			} else if(this->value[x] == '&') {
				do {
					std::string _VarnameCall = "";
					int i;
					for(i=this->value.find("&")+1; i<this->value[i]!=0; i++) {
						if(this->value[i] == ',' || this->value[i] == '.') break;
						else {
							_VarnameCall.push_back(this->value[i]);
						}
					}
					std::vector<std::string> __ENTRY__ = Variable::ValueOf(_VarnameCall, x, current_line);
					std::string _Tmp = replaceAll(this->value, "&"+__ENTRY__[0], __ENTRY__[1]);
					this->value = _Tmp;
					isMulMode = false;
					x = i - 1;
					mul_registry.clear();
				} while(this->value.find("&") != std::string::npos);
				x = -1;
			}
			
			else {
				decval.push_back(-1);
				error_pos = x, error_Ln = current_line;
				// break;
			}
		}

		std::vector<int> __Val;
		__Val.push_back(current_value);
		current_value = 0;
		return isVariableOnBuild ? __Val : decval;
	}
};

void INLINECOMPILER() {
	std::string input_string;
	std::cin >> input_string;
	print(Parser(input_string).Parse(false));
}

void FILECOMPILER(const char* filename) {
	int cur_Ln = 1;
	std::fstream FILE;
	FILE.open(filename, std::ios::in);
	std::vector<int> on_compile;
	if(FILE.is_open()) {
		std::string FILE_CONTENT_Ln;
		while(getline(FILE, FILE_CONTENT_Ln)) {
			lines.push_back(FILE_CONTENT_Ln);
			on_compile = Parser(FILE_CONTENT_Ln).Parse(cur_Ln);
			cur_Ln++;
		}
		FILE.close();
	}
	if(!isVariableOnBuild) print(CharBuilder(on_compile));
}

using namespace std;

int main(int argc, char* argv[]) {
	argc == 1 ? INLINECOMPILER() : argc > 2 ? print("too many arguments") : FILECOMPILER(argv[1]);
	// std::vector<std::string> vect;
	// std::cout << getIndex(vect, "ryve") << std::endl;
	// Variable::Status();
}

