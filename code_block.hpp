#ifndef CODE_BLOCK_HPP
#define CODE_BLOCK_HPP

#include <string>
#include <cstdint>
#include <unordered_set>
#include <vector>

using std::vector;
using std::string;
using std::uint64_t;
using std::unordered_set;

namespace binmark {

class linedata {
    string source_;
    uint64_t lineno_;
public:
    linedata(const string &source, uint64_t lineno): 
        source_(source), lineno_(lineno)
    {};

    const string &source() const 
    {
        return source_;
    }
    uint64_t lineno() const
    {
        return lineno_;
    }

    bool same_source(const linedata &o) const
    {
        return source_ == o.source_ && lineno_ == o.lineno_;
    }
};

class code_block: public linedata {
    uint64_t startinsno_;
    unordered_set<uint64_t> backtrace_;
    unordered_set<uint64_t> fwdptrs_;

public:
    code_block(uint64_t startinsno, const linedata &l): 
        linedata(l), startinsno_(startinsno), backtrace_(), fwdptrs_() 
    {};

    uint64_t startinsno() const
    {
        return startinsno_;
    }

    const unordered_set<uint64_t>& backtrace() const
    {
        return backtrace_;
    }

    const unordered_set<uint64_t>& fwdptrs() const
    {
        return fwdptrs_;
    }

    void link(uint64_t split_addr)
    {
        fwdptrs_.insert(split_addr);
    }

    void backlink(uint64_t split_addr)
    {
        backtrace_.insert(split_addr);
    }

    void doublelink(code_block &to, uint64_t split_addr);
    void handle_split(uint64_t split_addr, code_block &from);

};

code_block *find_block(uint64_t insno, code_block *beg, size_t sz);

code_block *find_block(vector<code_block> &blocks, uint64_t insno);

} // namespace

#endif // CODE_BLOCK_HPP
