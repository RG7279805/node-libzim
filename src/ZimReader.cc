#include <string>
#include <iostream>
#include <zim/file.h>
#include <zim/search.h>

#include "Article.cc"
#include "nbind/api.h"

class ZimReaderWrapper
{
  public:
    ZimReaderWrapper()
    {
    }

    zim::File *_reader;
};

class ZimReaderManager
{
  public:
    static ZimReaderWrapper *create(std::string fileName)
    {
        static ZimReaderWrapper rw;
        rw._reader = new zim::File(fileName);
        return (&rw);
    }

    static void getArticleByUrl(ZimReaderWrapper *rw, std::string url, nbind::cbFunction &callback)
    {
        try
        {
            zim::Article article = rw->_reader->getArticleByUrl(url);
            Article _article = getArticleFromZimArticle(article);

            callback(NULL, _article);
        }
        catch (...)
        {
            callback("Failed to find article");
            throw;
        }
    }

    static void suggest(ZimReaderWrapper *rw, std::string query, nbind::cbFunction &callback)
    {
        try
        {
            const zim::Search *search = rw->_reader->suggestions(query, 0, 10);
            zim::Search::iterator it = search->begin();
            std::string url = it.get_snippet();
            int results = search->get_matches_estimated();
            callback(NULL, results);
        }
        catch (...)
        {
            callback("Failed to search");
            throw;
        }
    }

    static void search(ZimReaderWrapper *rw, std::string query, nbind::cbFunction &callback)
    {
        try
        {
            const zim::Search *search = rw->_reader->search(query, 0, 10);
            zim::Search::iterator it = search->begin();
            std::string url = it.get_snippet();
            int results = search->get_matches_estimated();
            callback(NULL, results);
        }
        catch (...)
        {
            callback("Failed to search");
            throw;
        }
    }

    static Article getArticleFromZimArticle(zim::Article _article)
    {
        unsigned char *bufferData = (unsigned char *)_article.getData().data();
        size_t bufferLength = size_t(_article.getArticleSize());
        nbind::Buffer buf(bufferData, bufferLength);

        std::string ns = std::string(1, _article.getNamespace());

        // _article.getData().data
        // _article.getData().data()
        Article article(
            ns,
            "", // _article.getIndex(),
            _article.getUrl(),
            _article.getTitle(),
            _article.getMimeType(),
            "", // _article.getRedirectIndex(),
            "", // _article.getFilename(),
            buf);

        return article;
    }
};

#include "nbind/nbind.h"

#ifdef NBIND_CLASS

NBIND_CLASS(ZimReaderWrapper)
{
    construct();
}

NBIND_CLASS(ZimReaderManager)
{
    method(create);
    method(getArticleByUrl);
    method(suggest);
    method(search);
}

#endif
