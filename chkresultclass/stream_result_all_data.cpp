#include "stream_result_all_data.h"

stream_result_all_data::stream_result_all_data(const QString &testitem,const QString &testresult,const QString &wholeresult)
{

    this->m_testitem=testitem;
    this->m_testresult = testresult;
    this->m_wholeresult = wholeresult;
}
