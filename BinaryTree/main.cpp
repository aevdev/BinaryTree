//BinaryTree
#include <iostream>

using namespace std;

class Tree
{
protected:
	class Element
	{
		int Data;
		Element* pLeft;
		Element* pRight;
	public:
		Element(int Data, Element* pLeft = nullptr, Element* pRight = nullptr) : Data(Data), pLeft(pLeft), pRight(pRight)
		{
			cout << "EConstructor:\t" << this << endl;
		}
		~Element()
		{
			cout << "EDestructor:\t" << this << endl;
		}

		bool isLeaf()const
		{
			return pLeft == pRight; //pLeft может быть равен pRight только в одном случае - это когда они оба равны nulltpr;
		}
		friend class Tree;
		friend class UniqueTree;
	}*Root;
public:
	Element* getRoot()const
	{
		return Root;
	}
	Tree()
	{
		this->Root = nullptr;
		cout << "TConstructor:\t" << this << endl;
	}
	Tree(const std::initializer_list<int>& il) : Tree()
	{
		for (int const* it = il.begin(); it != il.end(); it++)
		{
			insert(*it, Root);
		}
	}
	Tree(const Tree& other)
	{
		copy(other.Root);
		cout << "CopyConstructor:\t" << this << endl;
	}
	~Tree()
	{
		clear();
		cout << "TDestructor:\t" << this << endl;
	}

	//wrapper'ы (или "обертки") это функции, которые будут вызывать другие функции, их мы пишем для того, чтобы, в нашем случае, не прописывать постоянно tree.getRoot() в main'е.
	//так же это используется для того, чтобы соблюсти инкапсуляцию, ибо если у нас в мейне будет два дерева, то если у нас функции будут доступны из public секции,
	//то мы сможем для объекта одного дерева вызвать функцию и передать в нее корень другого дерева, что нарушает инкапсуляцию. То, что относится к отдельному объекту, не должно задевать другие объекты.
	void insert(int Data)
	{
		insert(Data, Root);
	}
	void erase(int Data)
	{
		erase(Data, Root);
	}
	void clear()
	{
		clear(Root);
		Root = nullptr;
	}
	int minValue()const
	{
		return minValue(Root);
	}
	int maxValue()const
	{
		return maxValue(Root);
	}
	void print()const
	{
		print(Root);
		cout << endl;
	}
	int size()const
	{
		return size(Root);
	}
	int sum()const
	{
		return sum(Root);
	}
	double avg()const
	{
		return (double)sum(Root) / size(Root);
	}
	bool isTree()const
	{
		return isTree(Root);
	}
private:
	//функции в private секции, для которых существуют wrapper'ы(обертки) называются движками. 
	void insert(int Data, Element* Root)
	{
		if (this->Root == nullptr)
		{
			this->Root = new Element(Data);
			return;
		}
		if (Root == nullptr) return;
		if (Data < Root->Data)
		{
			if (Root->pLeft == nullptr)
				Root->pLeft = new Element(Data);
			else insert(Data, Root->pLeft);
		}
		else
		{
			if (Root->pRight == nullptr)
				Root->pRight = new Element(Data);
			else insert(Data, Root->pRight);
		}
	}

	void erase(int Data, Element*& Root)
	{
		if (Root == nullptr) return;
		erase(Data, Root->pLeft);
		erase(Data, Root->pRight);
		if (Data == Root->Data)
		{
			if (Root->isLeaf())
			{
				delete Root;
				Root = nullptr;
			}
				//при удалении элементов желательно подменять удаленный элемент элементом из той ветки, что тяжелее, так у нас дерево при удалении элементов будет автоматически балансироваться
			else//ну и мы хотим сохранять его сбалансированным.
			{
				if (size(Root->pLeft) > size(Root->pRight))
				{
					Root->Data = maxValue(Root->pLeft);
					erase(maxValue(Root->pLeft), Root->pLeft);
				}
				else
				{
					Root->Data = minValue(Root->pRight);
					erase(minValue(Root->pRight), Root->pRight);
				}
			}
		}	

		

	}

	void clear(Element* Root)
	{
		if (Root == nullptr)return;
		clear(Root->pLeft);
		clear(Root->pRight);
		delete Root;
	}

	void copy(Element* Root)
	{
		if (Root == nullptr) return;
		insert(Root->Data, this->Root);
		copy(Root->pLeft);
		copy(Root->pRight);
	}

	int minValue(Element* Root)const
	{
		if (Root == nullptr) return 0;
		/*else if (Root->pLeft == nullptr) return Root->Data;						//// 1 Вариант
		else return minValue(Root->pLeft);*/

		//return Root->pLeft == nullptr ? Root->Data : minValue(Root->pLeft);		//// 2 Вариант

		return Root->pLeft ? minValue(Root->pLeft) : Root->Data;					//// 3 Вариант
	}
	int maxValue(Element* Root)const
	{
		if (Root == nullptr) return 0;
		/*else if (Root->pRight == nullptr) return Root->Data;						//// 1 Вариант
		else return minValue(Root->pRight);*/

		//return Root->pRight == nullptr ? Root->Data : minValue(Root->pRight);		//// 2 Вариант

		return Root->pRight ? maxValue(Root->pRight) : Root->Data;					//// 3 Вариант
	}
	int size(Element* Root)const
	{
		return Root == nullptr ? 0 : size(Root->pLeft) + size(Root->pRight) + 1;
	}
	int sum(Element* Root)const
	{
		return !Root ? 0 : sum(Root->pLeft) + sum(Root->pRight) + Root->Data; //!Root = (Root == nullptr)	
	}
	bool isTree(Element* Root)const
	{
		if (Root == nullptr) return false;
		if (Root->pLeft)
		{
			if (Root->Data < Root->pLeft->Data || isTree(Root->pLeft) == false)
				return false;
		}
		else if (Root->pRight)
		{
			if (Root->Data < Root->pRight->Data || isTree(Root->pRight) == false)
				return false;
		}
		return true;
	}
	void print(Element* Root)const
	{
		if (Root == nullptr) return;
		print(Root->pLeft);
		cout << Root->Data << "\t";
		print(Root->pRight);
	}
};

class UniqueTree : public Tree
{
private:
	void insert(int Data, Element* Root)
	{
		if (this->Root == nullptr)
		{
			this->Root = new Element(Data);
			return;
		}
		if (Root == nullptr) return;
		if (Data < Root->Data)
		{
			if (Root->pLeft == nullptr)
				Root->pLeft = new Element(Data);
			else insert(Data, Root->pLeft);
		}
		else if (Data > Root->Data)
		{
			if (Root->pRight == nullptr)
				Root->pRight = new Element(Data);
			else insert(Data, Root->pRight);
		}
	}
public:
	void insert(int Data)
	{
		insert(Data, this->Root);
	}
};

//#define BASE_CHECK
//#define TREE_CHECK

int factorial(int n)
{
	return n ? factorial(n - 1)* n : 1;
}

double power(double a, int n)
{
	return n ? power(a, n - 1) * a: 1;
}
int main()
{
#ifdef BASE_CHECK
	int n;
	cout << "Input tree size: ";
	cin >> n;
	UniqueTree tree;
	for (int i = 0; i < n; ++i)
	{
		tree.insert(rand() % 100 + 1);
	}
	//tree.clear();
	tree.print();
	cout << endl;
	cout << "Min value in tree: " << tree.minValue() << endl;
	cout << "Max value in tree: " << tree.maxValue() << endl;
	cout << "Size of tree: " << tree.size() << endl;
	cout << "Sum of elements: " << tree.sum() << endl;
	cout << "Average: " << tree.avg() << endl;
#endif //BASE_CHECK
#ifdef TREE_CHECK
	int value;
	Tree tree = { 50, 25, 75, 16, 32, 64, 80, 27, 35 };
	tree.print();
	Tree other = tree;
	other.print();
	if (tree.isTree())
		cout << "\nIs tree!";
	else
		cout << "\nIs not tree!";
	cout << "\nPlease, enter element you want to erase: "; cin >> value;
	tree.erase(value);
	tree.print();
#endif // TREE_CHECK
	cout << factorial(4) << '\n';
	cout << power(3, 3);
	return 0;
}