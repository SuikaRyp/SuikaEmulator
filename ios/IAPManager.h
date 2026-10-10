#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#include "../suikaemulator_config.h"

#if SuikaEmulator_PLATFORM(IOS_APP_STORE)

@interface IAPManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>

+ (instancetype)sharedIAPManager;

- (void)buyGoldWithRequestID:(int)requestID;
- (void)restorePurchasesWithRequestID:(int)requestID;
- (BOOL)isGoldUnlocked;
- (void)startObserving;
- (void)updateIcon:(bool)force;

@end

#endif
