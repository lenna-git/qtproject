#ifndef STREAM_RESULT_ALL_DATA_H
#define STREAM_RESULT_ALL_DATA_H
#include<QString>


class stream_result_all_data
{
public:
    stream_result_all_data(const QString &testitem,const QString &testresult,const QString &wholeresult );
    ~stream_result_all_data();


    QString m_testitem;
    QString m_testresult;
    QString m_wholeresult;
};

#endif // STREAM_RESULT_ALL_DATA_H
