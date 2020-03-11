#ifndef MYBLOCK_H
#define MYBLOCK_H


class myBlock
{
    int start_sector_ID, total_sectors;
    friend class FileSystem;
    friend class systemWindow;
public:
    myBlock()
    {
        start_sector_ID = -1;
        total_sectors = 0;
    }

    myBlock(int start, int sectors)
        :start_sector_ID(start), total_sectors(sectors)
    {}

    bool removeSectors(int n_of_secs)
    {
        if (n_of_secs >= total_sectors)
            return false;

        start_sector_ID += n_of_secs;
        total_sectors -= n_of_secs;
    }

    bool operator ==(const myBlock& obj)
    {
        if (start_sector_ID == obj.start_sector_ID && total_sectors == obj.total_sectors)
        {
            return true;
        }

        return false;
    }

    ~myBlock() {}
};


#endif // MYBLOCK_H
