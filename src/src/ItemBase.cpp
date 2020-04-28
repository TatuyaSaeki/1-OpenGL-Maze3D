#include "stdafx.h"


ItemBase::ItemBase()
: _texture(nullptr)
{

}

ItemBase::~ItemBase()
{
}

bool ItemBase::init()
{
	return true;
}

void ItemBase::update()
{
}

void ItemBase::render()
{
}

void ItemBase::cleanup()
{
	SAFE_DELETE(_texture);
}