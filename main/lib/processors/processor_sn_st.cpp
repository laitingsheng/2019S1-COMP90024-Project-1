#include "processors/processor_sn_st.h"

processor_sn_st::processor_sn_st(char const * filename, grid const & g) : processor(filename, g) {}

void processor_sn_st::preprocess()
{
    auto curr = file.data();
    process_block(curr, curr + file.size(), record);
}

processor::result_type processor_sn_st::operator()() const
{
    result_type re(g.count());
    for (unsigned int i = 0; i < g.count(); ++i)
    {
        // @formatter:off
        auto & [c, m] = record[i];
        auto & [rc, rv] = re[i];
        // @formatter:on
        rc = {i, c};
        if (c == 0)
            continue;
        rv.resize(m.size());
        auto it = rv.begin();
        for (auto & p : m)
        {
            *it = p;
            ++it;
        }
        std::sort(rv.begin(), rv.end(), less_tag_info);

        int vc = 0;
        unsigned long lc = 0;
        unsigned long j = 0;
        while (j < rv.size())
        {
            auto c = rv[j].second;
            if (c == 0)
            {
                lc = c;
                break;
            }
            if (c != lc)
            {
                ++vc;
                lc = c;
                if (vc > 5)
                    break;
            }
            ++j;
        }
        if (lc != 0 && vc > 5)
            rv.resize(j);
    }
    std::sort(re.begin(), re.end(), less_cell_total_info);
    return re;
}
