#ifndef PAINTSERVER_HPP
#define PAINTSERVER_HPP

namespace hpl {
class PaintServer
{

public:
    PaintServer();
    ~PaintServer();

    //! @todo probably return ids
    void addPoints(/*Points points*/);
    void addLines(/*Lines lines*/);
    void addContour(/*Contour contour*/);
    void addText(/*Text text*/);

private:
    //! @todo save this stuff

};
}

#endif // PAINTSERVER_HPP
