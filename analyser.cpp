
#include "analyser.hpp"

#include <vector>
#include <stack>
#include <set>
#include <string>

using binmark::code_block;
using std::vector;
using std::stack;
using std::set;
using binmark::parser;
using std::string;
using binmark::ast;

void analyse(FILE *file, ast &tree)
{
    parser p(file);

    vector<code_block> blocks;

    p.parse(tree, blocks);

    size_t num_checks = 0;

    /*
    for (auto &cb: blocks) {
        printf("%llx <- ", cb.startinsno());
        for (auto &b: cb.backtrace())
            printf("%llx ", b);
        printf("-> ");
        for (auto &f: cb.fwdptrs())
            printf("%llx ", f);
        printf("\n");
    }
    */

    for (auto &cb: blocks) {
        size_t chckkind;
        auto pos = cb.source().find("softboundcets.h");
        if (pos != string::npos) {
            switch (cb.lineno()) {
#define X(KINDNUM, FNAME, DESC, FROM, TO) \
            case FROM ... TO:\
                chckkind = 2 * KINDNUM +1;\
                ++num_checks; \
                break;
#include "functions.def"
#undef X
            default:
                continue;
            }
            stack<code_block *> dfs;
            set<uint64_t> visited;
            //printf("track %llx", cb.startinsno());
            for (auto &bp: cb.backtrace()) {
                //printf("try push %llx\n", bp);
                dfs.push(find_block(blocks, bp));
                //printf("push %llx\n", find_block(blocks, bp)->startinsno());
                visited.insert(bp);
            }
            while (!dfs.empty()) {
                code_block *src = dfs.top();
                //printf("pop %llx\n", src->startinsno());
                dfs.pop();
                if (src->source().find("softbound") == string::npos) {
                    //printf(" found %llx", src->startinsno());
                    tree[src->source()][src->lineno()][chckkind].insert(
                        src->startinsno()
                    );
                }
                else 
                    for (auto bbp: src->backtrace())
                        if (!visited.count(bbp)) {
                            //printf("try push %llx\n", bbp);
                            dfs.push(find_block(blocks, bbp));
                            //printf("push %llx\n", find_block(blocks, bbp)->startinsno());
                            visited.insert(bbp);
                        }
            }
            //printf("\n");
        }
    }
    //printf("Number of check code blocks in binary: %zu\n", num_checks);
}

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

