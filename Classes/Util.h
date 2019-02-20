#include <vector>
#include "cocos2d.h"

using std::vector;

class Util
{
public:
	Util();
	~Util();

	template <typename T>
	static bool contains(vector<T>* vector, T element)
	{
		if (vector)
		{
			auto it = std::find(vector->begin(), vector->end(), element);
			if (it != vector->end()) return true;
		}
		return false;
	}

	template <typename T>
	static void addIfUnique(vector<T>* vector, T element) 
	{
		if (vector)
		{
			auto it = std::find(vector->begin(), vector->end(), element);
			if (it == vector->end()) vector->push_back(element);
		}
	}

	template<typename T>
	static void removeIfContains(vector<T>* vector, T element)
	{
		if (vector)
		{
			auto it = std::find(vector->begin(), vector->end(), element);
			if (it != vector->end()) vector->erase(it);
		}
	}
};

