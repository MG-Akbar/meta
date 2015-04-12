/**
 * @file metadata_parser.cpp
 * @author Chase Geigle
 */

#include "index/metadata_parser.h"

namespace meta
{
namespace index
{

metadata_parser::metadata_parser(const std::string& filename,
                                 const metadata::schema& schema)
    : parser_{filename, "\n\t"}, schema_{schema}
{
    // nothing
}

std::vector<metadata::field> metadata_parser::next()
{
    std::vector<metadata::field> mdata;
    mdata.reserve(schema_.size());
    for (const auto& finfo : schema_)
    {
        if (!parser_.has_next())
            throw metadata::exception{"metadata input file ended prematurely"};
        auto str = parser_.next();

        switch (finfo.type)
        {
            case metadata::field_type::SIGNED_INT:
                mdata.emplace_back(static_cast<int64_t>(std::stol(str)));
                break;

            case metadata::field_type::UNSIGNED_INT:
                mdata.emplace_back(static_cast<uint64_t>(std::stoul(str)));
                break;

            case metadata::field_type::DOUBLE:
                mdata.emplace_back(std::stod(str));
                break;

            case metadata::field_type::STRING:
                mdata.emplace_back(std::move(str));
                break;
        }
    }
    return mdata;
}
}
}
