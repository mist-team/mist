#ifndef __INCLUDE_DIO_APP__
#define __INCLUDE_DIO_APP__


// Event Handler Object
class mist_builder : public FXApp
{
	// Macro for class hierarchy declarations
	FXDECLARE( mist_builder )

public:
	typedef FXApp base;

public:
	// �A�v���P�[�V�����ň����t�B���^�Q
	filter_list filters;

	// �摜����`�悷��ۂɕK�v�ƂȂ�O���[�o���ȕ`��p�����[�^

public:
	mist_builder( const FXString &name = "Application", const FXString &vendor = "FoxDefault" );

};


#endif	// __INCLUDE_DIO_APP__
