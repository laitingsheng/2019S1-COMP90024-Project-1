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
        re[i] = record_to_total_info(i);
    std::sort(re.begin(), re.end(), less_cell_total_info);
    return re;
}
