#include <vector>
#include "cocos2d.h"

using std::vector;

class Util
{
public:
	Util();
	~Util();

	//template <typename type>
	//static bool contains(vector<type> vector, type element);

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
	

	//template <typename type>
	//static void remove(vector<type>* vector, type element);
};

