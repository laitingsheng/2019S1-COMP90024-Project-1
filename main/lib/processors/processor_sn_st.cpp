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
    for (unsigned long i = 0; i < g.count(); ++i)
    {
        // @formatter:off
        auto & [c, m] = record[i];
        // @formatter:on
        auto & [rc, rv] = re[i];
        rc = {i, c};
        rv.resize(m.size());
        auto it = rv.begin();
        // @formatter:off
        for (auto & p : m)
        // @formatter:on
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
            auto c = rv[i].second;
            if (c == 0)
                break;
            if (c != lc)
            {
                ++vc;
                lc = c;
                if (vc > 5)
                    break;
            }
            ++j;
        }
    }
    std::sort(re.begin(), re.end(), less_cell_total_info);
    return re;
}
