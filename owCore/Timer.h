#pragma once

class Timer {
public:
	class Callback {
	private:
		typedef void(*timerProcType)(uint32_t timerId);
		timerProcType timerProc;

	public:
		Callback() {
		}

		Callback(timerProcType timerProc) {
			this->timerProc = timerProc;
		}

		virtual void operator()(uint32_t timerId) {
			(*timerProc)(timerId);
		}
	};

	template<class T>
	class ClassCallback : public Callback {
	private:
		T* classInstancePtr;
		typedef void(T::*classTimerProcType)(uint32_t timerId);
		classTimerProcType classTimerProc;

	public:
		ClassCallback(T* classPtr, classTimerProcType timerProc) {
			this->classInstancePtr = classPtr;
			this->classTimerProc = timerProc;
		}

		virtual void operator()(uint32_t timerId) {
			(classInstancePtr->(*classTimerProc))(timerId);
		}
	};

	// Запуск таймера
	void Start(uint32_t time, Callback* timerObj) {
		UINT_PTR _id = SetTimer(NULL, NULL, time, Timer::TimerProc);
		_timers[_id] = timerObj;
		id = _id;
		//printf("Таймер id = %d запущен.\n", id);
	}

	UINT_PTR getId() const {
		return id;
	}

	// Остановка таймера
	void Stop() {
		if (id != 0) {
			KillTimer(NULL, id);
			delete _timers[id];
			_timers.erase(id);
			//printf("Таймер id = %d остановлен.\n", id);
		}
		else
			printf("Вы пытайтесь удалить неинициализированный таймер!\n");
	}

	bool operator==(const UINT_PTR& timerId) {
		return id == timerId;
	}

	static void CALLBACK TimerProc(HWND hwnd, UINT msg, UINT_PTR timerId, DWORD dwTime) {
		_timers[timerId]->operator()(timerId);
	}

private:
	static std::map<UINT_PTR, Callback*> _timers;
	UINT_PTR id;
};

bool operator==(const UINT_PTR& timerId, const Timer& timer);

#define TIMER_PROC(_method) new Timer::Callback(&_method)
#define TIMER_CLASS_PROC(_class, _method) new Timer::ClassCallback<_class>(this, &_class::_method)
