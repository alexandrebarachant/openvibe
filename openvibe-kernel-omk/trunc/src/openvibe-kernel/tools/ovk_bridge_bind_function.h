#ifndef __OpenViBEKernel_Tools_Function_Binding_H__
#define __OpenViBEKernel_Tools_Function_Binding_H__

#define __BridgeBindFunc0__(_target, _result, _fname, _addarg) \
	_result _fname(void) _addarg { return _target._fname(); }
#define __BridgeBindFunc1__(_target, _result, _fname, _addarg, _p1type, _p1name) \
	_result _fname(_p1type _p1name) _addarg { return _target._fname(_p1name); }
#define __BridgeBindFunc2__(_target, _result, _fname, _addarg, _p1type, _p1name, _p2type, _p2name) \
	_result _fname(_p1type _p1name, _p2type _p2name) _addarg { return _target._fname(_p1name, _p2name); }
#define __BridgeBindFunc3__(_target, _result, _fname, _addarg, _p1type, _p1name, _p2type, _p2name, _p3type, _p3name) \
	_result _fname(_p1type _p1name, _p2type _p2name, _p3type _p3name) _addarg { return _target._fname(_p1name, _p2name, _p3name); }
#define __BridgeBindFunc4__(_target, _result, _fname, _addarg, _p1type, _p1name, _p2type, _p2name, _p3type, _p3name, _p4type, _p4name) \
	_result _fname(_p1type _p1name, _p2type _p2name, _p3type _p3name, _p4type _p4name) _addarg { return _target._fname(_p1name, _p2name, _p3name, _p4name); }
#define __BridgeBindFunc5__(_target, _result, _fname, _addarg, _p1type, _p1name, _p2type, _p2name, _p3type, _p3name, _p4type, _p4name, _p5type, _p5name) \
	_result _fname(_p1type _p1name, _p2type _p2name, _p3type _p3name, _p4type _p4name, _p5type _p5name) _addarg { return _target._fname(_p1name, _p2name, _p3name, _p4name, _p5name); }

#endif // __OpenViBEKernel_Tools_Function_Binding_H__
