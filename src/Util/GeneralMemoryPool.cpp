#include "GeneralMemoryPool.h"

namespace Sun {
	//this table is revised from UE4 

	int32 GeneralMemoryPool::blockSizeTable_[POOL_TABLE_SIZE] =
	{
		4, 8 ,12, 16, 20 ,24, 28, 32, 36, 40, 44, 48,52, 56, 60, 64,
		72, 80, 88, 96, 112, 128,
		160, 192, 224, 256, 288, 320, 384, 448,
		512, 576, 640, 704, 768, 896, 1024 - RESIDUE_SIZE
	};

	uint8 GeneralMemoryPool::blockSizeLookUp_[MAX_BLOCK_SIZE + 1];

	bool GeneralMemoryPool::blockSizeLookUpInitialized_ = false;

	GeneralMemoryPool::GeneralMemoryPool() {
		memset(freeLists_, 0, sizeof(freeLists_));

		if (blockSizeLookUpInitialized_ == false) {	
			uint8 j = 0;
			uint8 split = 0;
			for (int32 i = 1; i <= MAX_BLOCK_SIZE; ++i) {
				if (i <= blockSizeTable_[j]) {
					blockSizeLookUp_[i] = j;
				}
				else {
					j += 1;
					blockSizeLookUp_[i] = j;
				}
				if (i == sizeof(GBlock)) {
					split = j;
				}
			}
			//revise
			for (int32 i = 1; i <= sizeof(GBlock); ++i) {
				blockSizeLookUp_[i] = split;
			}
			blockSizeLookUpInitialized_ = true;
		}
		chunks_.reserve(8);
	}

}