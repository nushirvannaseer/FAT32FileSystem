#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "linkedList.h"
#include "myBlock.h"
#include "myFile.h"

#include <QString>
#include <cmath>

class FileSystem
{
    linkedList <myBlock> pool;//sorted linked list of available blocks
    linkedList <myFile> Files;//linked list of all files on the disk
    char* disk;
    int num_of_sectors,  size_of_sectors;
    int num_of_sectors_in_pool;
    friend class MainWindow;
    friend class systemWindow;

public:
    FileSystem(int sectornum, int sectorsize)
        :size_of_sectors(sectorsize), num_of_sectors(sectornum)
    {
        disk = new char[size_of_sectors * num_of_sectors];
        for (int i = 0; i < size_of_sectors * num_of_sectors; i++)
        {
            disk[i] = '\0';
        }
        num_of_sectors_in_pool=num_of_sectors;

        myBlock* temp = new myBlock(0, num_of_sectors);
        pool.insertAtStart(*temp);//initially storing all sectors in a block
    }

    bool saveFile(QString fname, char* fcontent, int fsize)
        {
            int current_pool_mem = num_of_sectors_in_pool * size_of_sectors;
            fsize++;//incrementing fsize to include null character
            //checks if there is enough space for the new file
            if (current_pool_mem < fsize)
            {
                //cout << endl << endl << "No Memory" << endl << endl;
                return false;
            }

            node <myFile>* file_iterator = Files.head;

            //goes through the file to see if there is already a file with the same name or not
            while (file_iterator != nullptr)
            {
                if (file_iterator->data.getName() == fname)
                {
                   // cout << endl << endl << "File already Exists" << endl << endl;
                    return false;
                }

                file_iterator = file_iterator->next;
            }

            myFile* newFile = new myFile(fname, fsize);//creating the new file to be inserted

            node <myBlock>* block_iterator = pool.head->next;
            float required_sectors = float(fsize) / float(size_of_sectors);
            //checking if the required number of sectors is whole or not
            if (required_sectors == 0 || floor(required_sectors) != required_sectors)
            {
                required_sectors++;
            }

            //typecasting the double to type int
            required_sectors = int(required_sectors);

            //making a dummy file size variable to keep track
            int dum_f_size = fsize;
            bool iterator_change1 = false;

            while (block_iterator != pool.tail)
            {
                //this block is executed if the sectors in the pool's block is less than the required number
                //the whole block is allotted to the file and we move on
                if (block_iterator->data.total_sectors < required_sectors)
                {
                    node <myBlock>* to_add = block_iterator;
                    block_iterator = block_iterator->next;
                    iterator_change1 = true;
                    pool.removeElement(to_add);
                    newFile->blocks_used.insertAtEnd(*to_add);
                    num_of_sectors_in_pool -= to_add->data.total_sectors;
                    //decreasing the dummy file size so we know how many more bytes are needed
                    dum_f_size -= to_add->data.total_sectors * size_of_sectors;
                    required_sectors -= to_add->data.total_sectors;
                    //the loop only breaks if dummy file size==0
                    if (dum_f_size == 0)
                    {
                        break;
                    }
                }

                //the whole block is allotted to the file and the function returns
                else if (block_iterator->data.total_sectors == required_sectors)
                {
                    node <myBlock>* to_add = block_iterator;
                    block_iterator = block_iterator->next;
                    pool.removeElement(to_add);
                    newFile->blocks_used.insertAtEnd(*to_add);
                    num_of_sectors_in_pool -= to_add->data.total_sectors;
                    required_sectors -= to_add->data.total_sectors;
                    break;
                }

                //a chunk is broken from the larger block and allotted to the file
                else
                {
                    myBlock* newBlock = new myBlock(block_iterator->data.start_sector_ID, required_sectors);
                    newFile->blocks_used.insertAtEnd(*newBlock);
                    num_of_sectors_in_pool -= required_sectors;
                    block_iterator->data.start_sector_ID += required_sectors;
                    block_iterator->data.total_sectors -= required_sectors;
                    break;
                }

                if (iterator_change1 == false)
                {
                    block_iterator = block_iterator->next;
                }

            }

            Files.insertAtEnd(*newFile);

            //this block writes the file data to the disk
            node <myBlock>* b_iterator2 = newFile->blocks_used.head->next;
            int k = 0;
            while (b_iterator2 != newFile->blocks_used.tail)
            {
                for (int i = b_iterator2->data.start_sector_ID;
                    i <= b_iterator2->data.start_sector_ID + b_iterator2->data.total_sectors; i++)
                {
                    for (int j = i * size_of_sectors; j < (i * size_of_sectors) + size_of_sectors; j++)
                    {
                        disk[j] = fcontent[k];
                        if (k == fsize - 1)
                        {
                            return true;
                        }
                        k++;
                    }
                }

                b_iterator2 = b_iterator2->next;
            }
        }

        //deletes the file user wants to delete
        bool deleteFile(QString fname)
        {
            node <myFile>* file_iterator = Files.head;
            bool file_found = false;
            //goes through the file to see if the file exists or not
            while (file_iterator != nullptr)
            {
                if (file_iterator->data.getName() == fname)
                {
                    file_found = true;
                    break;
                }

                file_iterator = file_iterator->next;
            }

            if (file_found == false)
            {
                return false;
            }

            //if file is found;
            node<myBlock>* block_iterator = file_iterator->data.blocks_used.head->next,
                * prev_block = file_iterator->data.blocks_used.head;

            if (pool.isEmpty() == true)
            {
                node<myBlock>* to_add = block_iterator;
                block_iterator = block_iterator->next;
                pool.insertAtEnd(*to_add);
                num_of_sectors_in_pool += to_add->data.total_sectors;
            }

            while (block_iterator != file_iterator->data.blocks_used.tail)
            {
                node <myBlock>* pool_iterator = pool.head,
                    * prev_pool = pool.head;
                bool change_iterator = false;

                while (pool_iterator != pool.tail)
                {
                    if (pool_iterator->data.start_sector_ID > block_iterator->data.start_sector_ID)
                    {
                        //merging contiguous sectors
                        if (pool_iterator->data.start_sector_ID ==
                            block_iterator->data.start_sector_ID + block_iterator->data.total_sectors)
                        {
                            pool_iterator->data.start_sector_ID = block_iterator->data.start_sector_ID;
                            pool_iterator->data.total_sectors += block_iterator->data.total_sectors;
                            num_of_sectors_in_pool += block_iterator->data.total_sectors;
                            file_iterator->data.blocks_used.removeElement(block_iterator);

                            prev_block = block_iterator;
                            block_iterator = block_iterator->next;
                            delete prev_block;

                            prev_block = block_iterator;
                            change_iterator = true;
                        }

                        //if the sectors are not contiguous
                        else
                        {
                            num_of_sectors_in_pool += block_iterator->data.total_sectors;
                            prev_pool->next = block_iterator;
                            block_iterator = block_iterator->next;
                            prev_block = block_iterator;
                            prev_pool->next->next = pool_iterator;

                            change_iterator = true;
                        }

                        break;

                    }

                    else if (pool_iterator->next == pool.tail && pool_iterator->data.start_sector_ID <
                             block_iterator->data.start_sector_ID)
                    {
                        node<myBlock>* to_add = block_iterator;
                        block_iterator = block_iterator->next;
                        pool.insertAtEnd(*to_add);
                        num_of_sectors_in_pool += to_add->data.total_sectors;
                        prev_block = block_iterator;

                        change_iterator = true;
                        break;
                    }

                    prev_pool = pool_iterator;
                    pool_iterator = pool_iterator->next;
                }

                if (change_iterator == false)
                {
                    prev_block = block_iterator;
                    block_iterator = block_iterator->next;
                }

            }


            //this block checks if there are some inconsistencies in the pool
            node<myBlock>* pool_iterator = pool.head->next;
            node<myBlock>* prev_pool = pool.head;

            while (pool_iterator != pool.tail)
            {
                if (pool_iterator->data.start_sector_ID + pool_iterator->data.total_sectors == pool_iterator->next->data.start_sector_ID)
                {
                    pool_iterator->next->data.start_sector_ID -= pool_iterator->data.total_sectors;
                    pool_iterator->next->data.total_sectors += pool_iterator->data.total_sectors;
                    pool.removeElement(pool_iterator);
                    delete pool_iterator;
                    pool_iterator = prev_pool->next;
                }

                else
                {
                    prev_pool = pool_iterator;
                    pool_iterator = pool_iterator->next;
                }

            }

            Files.removeElement(file_iterator);
            file_iterator->data.blocks_used.head->next = file_iterator->data.blocks_used.tail;
            delete file_iterator;
            return true;

        }

    bool readFile(QString fname, QString &file_on_disk)
    {
        node <myFile>* file_iterator = Files.head;
        bool exists = false;
        //goes through the file to see if the file exists or not
        while (file_iterator != nullptr)
        {
            if (file_iterator->data.getName() == fname)
            {
                exists = true;
                break;
            }

            file_iterator = file_iterator->next;
        }

        if (exists == false)
        {
            //cout << "file does not exist" << endl;
            return false;
        }


        int fsize = file_iterator->data.size;

        node <myBlock>* b_iterator = file_iterator->data.blocks_used.head->next;
        while (b_iterator != file_iterator->data.blocks_used.tail)
        {
            for (int i = b_iterator->data.start_sector_ID*size_of_sectors;
                disk[i]!='\0'; i++)
            {
                //cout << disk[i];
                file_on_disk.append(disk[i]);
            }
            b_iterator = b_iterator->next;
        }

        //cout << endl;
        return true;

    }

    void printFileList()
    {
        if (Files.head == Files.tail)
        {
            //cout << "No files on disk" << endl;
            return;
        }

        node<myFile>* iterator = Files.head->next;
        while (iterator != Files.tail)
        {
            //cout << iterator->data.name << endl;
            iterator = iterator->next;
        }
    }

    ~FileSystem()
    {
        delete[]disk;
        node<myBlock>* prev = pool.head;
        node<myBlock>* curr_pointer = pool.head->next;

        while (curr_pointer != nullptr)
        {
            prev = curr_pointer;
            curr_pointer = curr_pointer->next;
            delete prev;
        }

        node<myFile>* prev2 = Files.head;
        node<myFile>* curr_pointer2 = Files.head->next;

        while (curr_pointer2 != nullptr)
        {
            prev2 = curr_pointer2;
            curr_pointer2 = curr_pointer2->next;
            delete prev2;
        }
    }
};



#endif // FILESYSTEM_H
