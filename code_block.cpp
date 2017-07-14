#include "code_block.hpp"

namespace binmark {

void code_block::doublelink(code_block &to, uint64_t split_addr)
{
    link(split_addr);
    to.backlink(startinsno());
}

void code_block::handle_split(uint64_t split_addr, code_block &from)
{
    startinsno_ = split_addr;
    this[-1].fwdptrs_ = {startinsno_};
    backtrace_ = {this[-1].startinsno_, from.startinsno_};
    from.fwdptrs_.insert(split_addr);

    auto search = this[1].backtrace_.find(this[-1].startinsno_);

    if (search != this[1].backtrace_.end()) {
        this[1].backtrace_.erase(search);
        this[1].backtrace_.insert(
            this[-1].startinsno_
        );
    }
}

code_block *find_block(uint64_t insno, code_block *beg, size_t sz)
{
    if (sz == 1)
        return beg;
    if (insno < beg[sz/2 +1].startinsno())
        return find_block(insno, beg, sz/2);
    else
        return find_block(insno, beg + sz/2, sz - sz/2);
}

code_block *find_block(vector<code_block> &blocks, uint64_t insno)
{
    return find_block(
        insno, blocks.begin().base(), blocks.size()
    );
};



} // namespace