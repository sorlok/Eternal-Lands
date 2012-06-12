#include "bookshelf.hpp"

#include <cstddef>
#include <cstdio>

#include "elloggingwrapper.h"

namespace  {

//Manage memory outside of the "book" struct.
class BookShelfItem {
public:
	book* item;
	BookShelfItem* left;  //ID is less
	BookShelfItem* right; //ID is greater

	BookShelfItem(book* item) : item(item), left(NULL), right(NULL) {
		if (!item) {
			LOG_ERROR("Attempting to create a null book (this will mess up the search algorithm).\n");
		}
	}
};

class Bookshelf {
public:
	BookShelfItem* root;

	Bookshelf() : root(NULL) {}

	~Bookshelf();
};


//Holds all books.
Bookshelf book_shelf;


//Helper: either add the book or advance the pointer. Return true if added.
bool add_or_descend(BookShelfItem*& parent, BookShelfItem*& child, book* newBook) {
	if (!child) {
		child = new BookShelfItem(newBook);
		return true;
	} else {
		parent = child;
		return false;
	}
}

//Helper: delete this book, its associated BookshelfItem, and its children
//NOTE: I copied most of this function from books.c with only minimal checking.
//      It seems to work, and valgrind doesn't complain about memory errors, but
//      it could use a manual read. 
void delete_books(BookShelfItem* curr) {
	if (!curr) { return; }

	//Delete children
	delete_books(curr->left);
	delete_books(curr->right);

	//Delete the book.
	book* b = curr->item;
	if (b) {
		//First free each page.
		page** pgs = b->pages;
		for(int i=0; i<b->no_pages; i++,pgs++) {
			page* p = *pgs;
			if(p->image) { free(p->image); }
			char **lines = p->lines;
			for(;*lines;lines++) { free(*lines); }
			free(p->lines);
			free(p);
		}

		//Now free all pages and the book itself.
		free(b->pages);
		free(b);
	}

	//Delete the item
	delete curr;
}

//Helper: Used for bookshelf printing (print_books)
void print_spaces(int num) {
	for (int i=0; i<num; i++) { printf(" "); }
}

//Debug: print the book shelf
void print_books(BookShelfItem* curr, int tabCount) {
	if (!curr) { return; }
	printf("%s\n", curr->item->title);
	if (curr->left) {
		print_spaces(tabCount*2);
		printf("LEFT: ");
		print_books(curr->left, tabCount+1);
	}
	if (curr->right) {
		print_spaces(tabCount*2);
		printf("RIGHT: ");
		print_books(curr->right, tabCount+1);
	}
}


//Bookshelf's actual destructor is defined here.
Bookshelf::~Bookshelf() {
	//Reclaim all books
	delete_books(book_shelf.root);
}



} //End un-named namespace


extern "C" void add_book(book* newBook)
{
	//Simple case
	if (!book_shelf.root) {
		book_shelf.root = new BookShelfItem(newBook);
		return;
	}

	//If there's at least one book.
	BookShelfItem* res = book_shelf.root;
	for (;;) {
		//Left is less, right is more
		if (newBook->id < res->item->id) {
			if (add_or_descend(res, res->left, newBook)) { break; }
		} else if (newBook->id > res->item->id) {
			if (add_or_descend(res, res->right, newBook)) { break; }
		} else {
			LOG_ERROR("Duplicate book id (%d), titles are:\n  %s\n  %s\n", res->item->id, res->item->title, newBook->title);
			return;
		}
	}
}


extern "C" book* get_book(int id)
{
	//Descend until we find it (or null)
	BookShelfItem* res = book_shelf.root;
	for (;;) {
		//Nothing left to search?
		if (!res) { break; }

		//Left is less, right is more
		if (id < res->item->id) {
			res = res->left;
		} else if (id > res->item->id) {
			res = res->right;
		} else {
			return res->item;
		}
	}
	LOG_WARNING("Can't find book with id: %d\n", id);
	return NULL;
}



