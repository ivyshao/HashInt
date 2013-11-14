#import "HttpModel.h"
#import "Constants.h"
#import "Utils.h"
#import "HTTPService.h"
#import "DataController.h"
#import "ASIHTTPRequest.h"
#import "AppDelegate.h"

@interface HttpModel()
@property (nonatomic, assign) BOOL isFetching;
@end
@implementation HttpModel
- (void)dealloc {
    self.data = nil;
    [super dealloc];
}

- (void)loadData {
    if (self.isFetching) {
        return;
    }
    if (self.data) {
        return;
    }
    self.isFetching = YES;
    [self _loadData];
}

- (void)onComplete:(NSNotification*)notif {
    self.isFetching = NO;
    if (notif.userInfo) {
        NSError *error = notif.userInfo[@"error"];
        [self handleError:error];
    } else {
        [self handleData:notif];
    }
}

- (void)handleError:(NSError *)error {
    if (error.code == ASIRequestTimedOutErrorType ||
        error.code == ASIConnectionFailureErrorType) {
        AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
        [appDelegate showTimeoutAlert];
    }
}

- (void)_loadData {
    
}

- (void)handleData:(NSNotification*)notif {
    
}
@end

@implementation HttpGetModel
- (void)_loadData {
    [Utils addObserver:self
              selector:@selector(onComplete:)
                  name:[self url]
                object:nil];
    [[HTTPService shared] getAPIData:[self url]];
}

- (NSString *)url {
    return nil;
}

@end

@implementation HttpGetArrayModel
- (void)handleData:(NSNotification*)notif {
    NSString *responseString = notif.object;
    NSArray *info = nil;
    @try {
        info = [Utils parseJsonStrToArray:responseString];
    }
    @catch (NSException *exception) {
        info = nil;
    }
    if (info) {
        self.data = info;
        [self _handleData];
    }
}

- (void)_handleData {
}
@end

@implementation HttpGetDictModel
- (void)handleData:(NSNotification*)notif {
    NSString *responseString = notif.object;
    NSDictionary *info = nil;
    @try {
        info = [Utils parseJsonStrToDict:responseString];
    }
    @catch (NSException *exception) {
        info = nil;
    }
    if (info) {
        self.data = info;
        [self _handleData];
    }
}

- (void)_handleData {
}
@end

@implementation LoginHttpModel

- (void)_loadData {
    NSString *url = [NSString stringWithFormat:@"%@%@", REQUEST_ROOT_URL, POST_USERS_LOGIN];
    [Utils addObserver:self
              selector:@selector(onComplete:)
                  name:url
                object:nil];
    NSDictionary *loginInfo = [[NSUserDefaults standardUserDefaults] objectForKey:USER_DEFAULTS_USER_LOGIN_INFO];
    [[HTTPService shared] postAPI:url
                             data:loginInfo
                         userInfo:nil];
}

- (void)handleData:(NSNotification*)notif {
    NSString *responseString = notif.object;
    NSDictionary *info = nil;
    @try {
        info = [Utils parseJsonStrToDict:responseString];
    }
    @catch (NSException *exception) {
        info = nil;
    }
    BOOL loggedIn = NO;
    if (info && [info isKindOfClass:[NSDictionary class]]) {
        if ([info objectForKey:@"loggedIn"]) {
            loggedIn = YES;
            self.data = info;
            [[DataController instance] handleLoginData:info];
        }
    }
    if (!loggedIn) {
        [[NSNotificationCenter defaultCenter] postNotificationName:NOTIFICATION_SHOW_LOGIN_PAGE
                                                            object:nil];
    }
}
@end


@implementation CurrenciesHttpModel
- (void)_handleData {
    [DataController instance].currencies = self.data;
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@", REQUEST_ROOT_URL, REQUEST_GET_CURRENCIES];
}
@end

@implementation CategoriesHttpModel
- (void)_handleData {
    [DataController instance].dealCategories = self.data;
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIFICATION_CATEGORIES_UPDATED
                                                        object:nil];
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@", REQUEST_ROOT_URL, REQUEST_CATEGORIES_GET];
}
@end

@implementation CountriesHttpModel
- (void)_handleData {
    [DataController instance].storeCountries = self.data;
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@", REQUEST_ROOT_URL, REQUEST_STORES_COUNTRIES];
}
@end

@implementation FollowersHttpModel
- (void)_handleData {
    [DataController instance].followers = self.data;
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIFICATION_SELF_FOLLOWERS_GOT
                                                        object:nil];
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@%@", REQUEST_ROOT_URL, REQUEST_USERS_FOLLOWERS, [DataController userId]];
}
@end

@implementation FollowingHttpModel
- (void)_handleData {
    [[DataController instance].storesFollowing updateFollowing:[Utils convertToArray:[self.data objectForKey:@"stores"]]];
    [[DataController instance].friendsFollowing updateFollowing:[Utils convertToArray:[self.data objectForKey:@"users"]]];
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIFICATION_SELF_FOLLOWING_GOT
                                                        object:nil];

}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@%@", REQUEST_ROOT_URL, REQUEST_USERS_FOLLOWING, [DataController userId]];
}
@end

@implementation MyStoresHttpModel
- (void)_handleData {
    [[DataController instance] updateMyStores:self.data];
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@/user_id/%@", REQUEST_ROOT_URL, REQUEST_STORES_MYSTORES, [DataController userId]];
}
@end

@implementation LikedDealsHttpModel

- (void)_handleData {
    [[DataController instance].likedDeals updateDeals:self.data];
}

- (NSString *)url {
    return [NSString stringWithFormat:@"%@%@%@", REQUEST_ROOT_URL, REQUEST_USERS_LIKE, [DataController userId]];
}
@end