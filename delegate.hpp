#pragma once
#ifndef SERD_DELEGATE_H
#define SERD_DELEGATE_H

// small changes to the main idea at the link
// http://www.codenet.ru/progr/visualc/events/

namespace serd {

	class DELEGATE {
		typedef bool ReturnType;
	public:
		DELEGATE() : mObj(nullptr), mFunc(nullptr) { };

		template <class Owner, class... Args>
		inline DELEGATE(Owner* owner, ReturnType(Owner::* func)(Args...)) : mObj(nullptr), mFunc(nullptr) {
			mObj  = reinterpret_cast<OBJ*>(owner);
			mFunc = reinterpret_cast<FUNC>(func);
		};

		template <class... Args>
		inline ReturnType operator() (Args... args) {
			typedef ReturnType(OBJ::* TFUNC)(Args...);
			TFUNC func = reinterpret_cast<TFUNC>(mFunc);
			bool result = false;

			if (mObj != nullptr && mFunc != nullptr)
				result = (mObj->*func)(args...);

			return result;
		};
	private:
		struct OBJ {};
		typedef ReturnType(OBJ::*FUNC)();
		OBJ* mObj;
		FUNC mFunc;
	};

};

#endif /* SERD_DELEGATE_H */
