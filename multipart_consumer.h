#ifndef MULTIPARTCONSUMER_H
#define MULTIPARTCONSUMER_H

#include "multipart_parser.h"

#include <QByteArray>
#include <QList>
#include <QMap>

class MultipartConsumer
{

  QList<QMap<QByteArray,QByteArray>> headers_;
  QByteArray last_header_name;
  QList<QByteArray> parts_;

public:
    MultipartConsumer(const QByteArray& boundary)
    {
        memset(&m_callbacks, 0, sizeof(multipart_parser_settings));
        m_callbacks.on_header_field = ReadHeaderName;
        m_callbacks.on_header_value = ReadHeaderValue;
        m_callbacks.on_part_data = ReadPartData;

        m_parser = multipart_parser_init(boundary.constData(), &m_callbacks);
        multipart_parser_set_data(m_parser, this);
    }

    ~MultipartConsumer()
    {
        multipart_parser_free(m_parser);
    }

    void exec(const QByteArray& body) {
        multipart_parser_execute(m_parser, body.constData(), body.size());
    }

    const QList<QMap<QByteArray,QByteArray>>& headers() const {
      return headers_;
    }

    const QList<QByteArray>& parts() const {
      return parts_;
    }

private:
    static int ReadHeaderName(multipart_parser* p, const char *at, size_t length)
    {
        MultipartConsumer* me = (MultipartConsumer*) multipart_parser_get_data(p);

        me->last_header_name = QByteArray(at, length);
        return 0;
    }

    static int ReadHeaderValue(multipart_parser* p, const char *at, size_t length)
    {
        MultipartConsumer* me = (MultipartConsumer*) multipart_parser_get_data(p);

        if (me->last_header_name.size() > 0) {
          const auto value = QByteArray(at, length);

          if (value.size() > 0) {
            if (me->headers_.size() <= me->parts().size())
              me->headers_.append(QMap<QByteArray,QByteArray>());
            me->headers_[me->parts().size()].insert(me->last_header_name, value);
          }

          me->last_header_name.clear();
        }
        return 0;
    }

    static int ReadPartData(multipart_parser* p, const char *at, size_t length) {
      MultipartConsumer* me = (MultipartConsumer*) multipart_parser_get_data(p);

      if (length > 0) {
        me->parts_.append(QByteArray(at, length));
      }

      return 0;
    }

    multipart_parser* m_parser;
    multipart_parser_settings m_callbacks;
    int m_headers;
};

#endif // MULTIPARTCONSUMER_H
