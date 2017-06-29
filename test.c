#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <stdbool.h>
#include "table.h"
#include "hash.h"

int main(void)
{
	/* code */
	Table* t = create(longHash, longEquals, longStrPrint);
	dump(t, true);
	destroy(t);
	return 0;
}
	
