#import <Foundation/Foundation.h>
#import <Virtualization/Virtualization.h>

#ifdef __arm64__
#define MACOS_GUEST 1
#endif

@interface VMSpec : VZVirtualMachineConfiguration {
    NSData  *machineIdentifierData, *hardwareModelData;
    NSArray *storage;
    /* type: disk / aux / initrd
       file: / url:
       readOnly: true/false */
    NSArray *displays;
    /* width:  height:  dpi: */
    NSArray *networks;
    /* type: */
    NSString *os;
    /* macos / linux */
    NSDictionary *bootInfo;
    /* Linux: kernel, parameters */

    NSString *ptyPath; /* internally generated */
@public
    int cpus;
    unsigned long ram;
    BOOL audio, use_serial, pty;
}

#ifdef MACOS_GUEST
@property (strong) VZMacOSRestoreImage *restoreImage;
#endif

- (instancetype) init;
- (NSError *) readFromJSON: (NSInputStream*) jsonStream;
- (NSError*) writeToJSON: (NSOutputStream*) jsonStream;
- (void) addFileStorage: (NSString*) path type: (NSString*) type readOnly: (BOOL) ro;
- (void) addFileStorage: (NSString*) path type: (NSString*) type options: (NSArray*) options;
- (void) addDefaults;
- (void) addDisplayWithWidth: (int) width height: (int) height dpi: (int) dpi;
- (void) addNetwork: (NSString*) type;
- (void) addNetwork: (NSString*) type interface: (NSString*) iface;
- (instancetype) configure;
- (void) cloneAllStorage;

@end

@interface VMInstance : NSObject
{
    @public
    dispatch_queue_t queue;
//    VZMacOSInstaller *installer;
}

@property (strong) VZVirtualMachine *virtualMachine;
@property (strong) VMSpec *spec;

- (instancetype) initWithSpec: (VMSpec*) spec;
- (void) start;
- (BOOL) stop;
- (void) performVM: (id) target selector: (SEL) aSelector withObject:(nullable id)anArgument;

@end
