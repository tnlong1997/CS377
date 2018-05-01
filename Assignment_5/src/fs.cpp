#include "fs.h"

using namespace std;
char disk_name[16];
fstream myDisk;

myFileSystem::myFileSystem(char diskName[16]) {
  // open the file with the above name
  // this file will act as the "disk" for your file system
    
    fstream disk(diskName);
    
    if (disk.good()) {
        myDisk.open(diskName, ios::out | ios::app);
        int i = 0;
        while (i < 16) {
            disk_name[i] = diskName[i];
            i = i + 1;
        }
    }
}

int myFileSystem::create_file(char name[8], int size) {
  //create a file with this name and this size

  // high level pseudo code for creating a new file

  // Step 1: Check to see if we have sufficient free space on disk by
  //   reading in the free block list. To do this:
  // Move the file pointer to the start of the disk file.
  // Read the first 128 bytes (the free/in-use block information)
  // Scan the list to make sure you have sufficient free blocks to
  //   allocate a new file of this size
    int success, free_block;
    success = 1;
    free_block = 0;
    char *p = (char *) calloc(128, sizeof(char));
    fstream file(disk_name, ios::in|ios::out);
    file.seekg(0, file.beg);
    file.read(p, 128);
    
    int i = 0;
    while (i < 128) {
        if (p[i] == 0) {
            free_block = free_block + 1;
        } else printf("not free %d\n", i);
        i = i + 1;
    }
    
    printf("num of free_block%d\n", free_block);
    
    if(size > free_block){
        return -1;
    }
    
  // Step 2: we look for a free inode on disk
  // Read in an inode
  // Check the "used" field to see if it is free
  // If not, repeat the above two steps until you find a free inode
  // Set the "used" field to 1
  // Copy the filename to the "name" field
  // Copy the file size (in units of blocks) to the "size" field

    char *inode_p = (char *)calloc(48, sizeof(char));
    idxNode *inode;
    int address;
    
    i = 0;
    while (i < 16) {
        address = i * 48 + 128;
        file.seekg(address, file.beg);
        file.read(inode_p, 48);
        inode = (idxNode *) inode_p;
        
        if (inode->used == 0) break;
        if (inode->used == 1 && i == 15) {
           return -1;
        }
        
        i = i + 1;
    }
    
    printf("%s ", name);
    printf("%d\n", address);
    
    inode->used = 1;
    for(int j =0; j < strlen(name); j++) {
        inode->name[j] = name[j];
    }
    
    printf("check input name: %s\n", inode->name);
    inode->size = size;
  // Step 3: Allocate data blocks to the file
  // for(i=0;i<size;i++)
  // Scan the block list that you read in Step 1 for a free block
  // Once you find a free block, mark it as in-use (Set it to 1)
  // Set the blockPointer[i] field in the inode to this block number.
  // end for
    
    int temp = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 128; j++) {
            if (p[j] == 0) {
                p[j] = 1;
                if (temp < 8) {
                    inode->blockPointers[temp] = j;
                    temp = temp + 1;
                }
                break;
            }
        }
    }
    
  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode
    
    file.seekp(0, file.beg);
    file.write(p, 128);
    file.seekp(address,file.beg);
    file.write(inode_p, 48);
    file.close();
    return success;
    
} // End Create

int myFileSystem::delete_file(char name[8]){
  // Delete the file with this name

  // Step 1: Locate the inode for this file
  // Move the file pointer to the 1st inode (129th byte)
  // Read in an inode
  // If the inode is free, repeat above step.
  // If the inode is in use, check if the "name" field in the
  //   inode matches the file we want to delete. If not, read the next
  //   inode and repeat
    fstream file(disk_name, ios::in|ios::out);
    idxNode *inode;
    
    int success, address, i;
    success = -1;
    char *inode_p =  (char *) calloc(48,sizeof(char));
    
    i = 0;
    while (i < 16) {
        address = i * 48 + 128;
        file.seekg(address, file.beg);
        file.read(inode_p, 48);
        inode = (idxNode *) inode_p;
        if (inode->used == 1) {
            printf("inode name: %s--\n", inode->name);
            printf("check name: %s--\n", name);
            if (strcmp(inode->name, name) == 0) {
                success = 1;
                break;
            }
        }
        i = i + 1;
    }
  // Step 2: free blocks of the file being deleted
  // Read in the 128 byte free block list (move file pointer to start
  //   of the disk and read in 128 bytes)
  // Free each block listed in the blockPointer fields as follows:
  // for(i=0;i< inode.size; i++)
  // freeblockList[ inode.blockPointer[i] ] = 0;
    
    char *p = (char *)calloc(128, sizeof(char));
    
    file.seekg(0, file.beg);
    file.read(p, 128);
    
    for (int i = 0; i < inode->size; i++) {
        p[inode->blockPointers[i]] = 0;
    }
    
  // Step 3: mark inode as free
  // Set the "used" field to 0.
    
    inode->used = 0;
    
  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode
    
    file.seekp(0, file.beg);
    file.write(p, 128);
    file.seekp(address, file.beg);
    file.write(inode_p, 48);
    file.close();
    return success;
    
} // End Delete


int myFileSystem::ls(){
  // List names of all files on disk

  // Step 1: read in each inode and print
  // Move file pointer to the position of the 1st inode (129th byte)
  // for(i=0;i<16;i++)
  // Read in an inode
  // If the inode is in-use
  // print the "name" and "size" fields from the inode
  // end for

    fstream file(disk_name, ios::in|ios::out);
    idxNode *inode;
    
    int i = 0;
    
    while (i < 16) {
        char *inode_p =  (char *) calloc(48,sizeof(char));
        
        file.seekg (i * 48 + 128, file.beg);
        file.read (inode_p, 48);
        
        inode = (idxNode *) inode_p;
        
        if (inode->used == 1) {
            printf("%s", inode->name);
            printf(" Size of the inode: %d\n",inode->size);
            for(int c = 0; c < 8; c++) {
                printf("%d\n", inode->blockPointers[c] );
            }
        }
        
        i = i + 1;
    }
    return 0;
}; // End ls

int myFileSystem::read(char name[8], int blockNum, char buf[1024]){
  // read this block from this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in an inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat
    
    fstream file(disk_name, ios::in|ios::out);
    idxNode *inode;
    
    int success, address, i;
    success = -1;
    char *inode_p = (char *) calloc(48, sizeof(char));
    
    i = 0;
    
    while (i < 16) {
        address = i * 48 + 128;
        file.seekg(address, file.beg);
        file.read(inode_p, 48);
        
        inode = (idxNode *) inode_p;
        if (inode->used == 1) {
            if(strcmp(inode->name, name) == 0) {
                success = 1;
                break;
            }
        }
        i = i + 1;
    }
    
  // Step 2: Read in the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., to byte #
  //   addr*1024 in the file)
    
    if (blockNum >= inode->size) {
        return -1;
    }
    
    int addr = inode->blockPointers[blockNum];
    char *buff_p = (char *) calloc(1024, sizeof(char));
    file.seekg(addr * 1024,file.beg);
    file.read(buff_p, 1024);
    
  // Read in the block => Read in 1024 bytes from this location
  //   into the buffer "buf"
    
    i = 0;
    while (i < 1024) {
        buf[i] = buff_p[i];
        printf("%c", buf[i]);
        i = i + 1;
    }
    
    file.seekp(address,file.beg);
    file.write(inode_p, 48);
    file.close();
    
    return success;
    
} // End read


int myFileSystem::write(char name[8], int blockNum, char buf[1024]){
  // write this block to this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in a inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat
    
    fstream file(disk_name, ios::in|ios::out);
    idxNode *inode;
    
    int success, address, i;
    success = -1;
    
    char *inode_p = (char *) calloc(48, sizeof(char));
    i = 0;
    while (i < 16) {
        address = i * 48 + 128;
        file.seekg(address, file.beg);
        file.read(inode_p, 48);
        inode = (idxNode *) inode_p;
        if (inode->used == 1) {
            if (strcmp(inode->name, name) == 0) {
                success = 1;
                break;
            }
        }
        
        i = i + 1;
    }

    
  // Step 2: Write to the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., byte # addr*1024)
    
    if (blockNum >= inode->size) {
        return -1;
    }
    
    int addr = inode->blockPointers[blockNum];
    char *buff_p = (char *) calloc(1024, sizeof(char));
    file.seekg(addr * 1024, file.beg);
    file.read(buff_p, 1024);
    
  // Write the block! => Write 1024 bytes from the buffer "buff" to
  //   this location
    
    i = 0;
    while (i < 1024) {
        buff_p[i] = buf[i];
        printf("%c", buf[i]);
        i = i + 1;
    }
    
    file.seekp(address, file.beg);
    file.write(inode_p, 48);
    file.seekp(addr * 1024,file.beg);
    file.write(buff_p, 1024);
    file.close();
    
    return success;
} // end write

int myFileSystem::close_disk(){
    myDisk.close();
    return 0;
}
