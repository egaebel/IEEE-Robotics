bool fullOfBlocks(block[] blocks, int numBlocks) {

	for (int i = 0; i < numBlocks; i++) {

		if (blocks[i] == NULL) {

			return false;
		} 
	}
	return true;
}