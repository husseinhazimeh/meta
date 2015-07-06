/**
 * @file pivoted_length.cpp
 * @author Sean Massung
 */

#include "index/inverted_index.h"
#include "index/ranker/pivoted_length.h"
#include "index/score_data.h"
#include "util/fastapprox.h"

namespace meta
{
namespace index
{

const std::string pivoted_length::id = "pivoted-length";

pivoted_length::pivoted_length(float s) : s_{s}
{
    /* nothing */
}

float pivoted_length::score_one(const score_data& sd)
{
    float doc_len = sd.idx.doc_size(sd.d_id);
    float TF = 1.0f + fastapprox::fastlog(
                          1.0f + fastapprox::fastlog(sd.doc_term_count));
    float norm = (1.0f - s_) + s_ * (doc_len / sd.avg_dl);
    float IDF
        = fastapprox::fastlog((sd.num_docs + 1.0f) / (0.5f + sd.doc_count));
    return TF / norm * sd.query_term_weight * IDF;
}

template <>
std::unique_ptr<ranker>
    make_ranker<pivoted_length>(const cpptoml::table& config)
{
    auto s = pivoted_length::default_s;
    if (auto c_s = config.get_as<double>("s"))
        s = *c_s;
    return make_unique<pivoted_length>(s);
}
}
}
