/**
 * @file absolute_discount.h
 *
 * All files in META are released under the MIT license. For more details,
 * consult the file LICENSE in the root of the project.
 *
 * @author Sean Massung
 */

#ifndef META_ABSOLUTE_DISCOUNT_H_
#define META_ABSOLUTE_DISCOUNT_H_

#include "index/ranker/lm_ranker.h"
#include "index/ranker/ranker_factory.h"

namespace meta
{
namespace index
{

/**
 * Implements the absolute discounting smoothing method.
 *
 * Required config parameters:
 * ~~~toml
 * [ranker]
 * method = "absolute-discount"
 * ~~~
 *
 * Optional config parameters:
 * ~~~toml
 * delta = 0.7
 * ~~~
 */
class absolute_discount : public language_model_ranker
{
  public:
    /**
     * The identifier of this ranker.
     */
    const static std::string id;

    /**
     * @param delta
     */
    absolute_discount(float delta = 0.7f);

    /**
     * Calculates the smoothed probability of a term.
     * @param sd score_data for the current query
     */
    float smoothed_prob(const score_data& sd) const override;

    /**
     * A document-dependent constant.
     * @param sd score_data for the current query
     */
    float doc_constant(const score_data& sd) const override;

  private:
    /// the absolute discounting parameter
    const float delta_;
};

/**
 * Specialization of the factory method used to create absolute_discount
 * rankers.
 */
template <>
std::unique_ptr<ranker> make_ranker<absolute_discount>(const cpptoml::table&);
}
}
#endif
