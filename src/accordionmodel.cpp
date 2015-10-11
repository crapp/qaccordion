#include "qAccordion/accordionmodel.h"


AccordionModel::AccordionModel()
{
    this->currentlyOpen = nullptr;
}

void AccordionModel::setCurrentlyOpen(ContentPane *pane)
{
    this->currentlyOpen = pane;
}

ContentPane *AccordionModel::getCurrentlyOpen()
{
    return this->currentlyOpen;
}
